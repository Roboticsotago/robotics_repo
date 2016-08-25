// Fan control firmware for 2016 MIDIBots
// Possibly doesn't need to use MIDIBot library, but the MIDI control is handy for testing.

/*
TODO:
[ ] Add code to read from pressure sensor. Should take initial reading for "zero" reference.
[ ] Add code to control fan speed to maintain desired pressure. Have some sanity checks, e.g. spin down if fault detected/suspected.
https://www.youtube.com/watch?v=UR0hOmjaHp0
http://robotics.stackexchange.com/questions/10127/pid-with-position-and-velocity-goal/10128#10128
http://robotics.stackexchange.com/questions/5260/the-aerial-refueling-problem-sketch-of-a-feedback-controller
https://www.youtube.com/watch?v=H4YlL3rZaNw
https://www.youtube.com/watch?v=taSlxgvvrBM
*/


#include <MIDIBot.h>
MIDIBot fanBot;

const int PWM_FREQUENCY = 25000; // Intel 4-wire fan spec. The Delta fan datasheet indicates 30 kHz .. 300 kHz, however.

void setup() {
//	fan_speed(0);	// Bit dodgy setting this before the setup routine, but the fan spools up for some seconds otherwise.
	fanBot.begin();
}

void loop() {
	fanBot.process_MIDI();
}


// Tricky low-level code from Chris for programming the PWM output for precise frequencies...

// On the Mega, we have timer1 attached to pins D11 and D12, D12 being the primary one.
// On "ordinary" Arduinos, it's on pins 9 and 10.  On the MIDIBot shield, pin 10 is Servo 2 (middle pin).
// Ideally we'd use the PWM MOSFET output on D6, but that uses Timer 0 and doesn't support frequency-accurate PWM, as far as I can tell.
// This will need an external circuit anyway to limit the current and block DC, so it's not a huge hassle to add a MOSFET circuit (with resistors and protection diode) to this as well.  It can be powered from one of the aux 12 V headers.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
const int OUTPUT_PIN     = 12,
const int OUTPUT_PIN_AUX = 11;
#else
const int OUTPUT_PIN     = 10;
const int OUTPUT_PIN_AUX = 9;
#endif


// General settings:
const int prescale = 8;

// Some PWM control definitions:
#define TIMER_CLK_STOP		0x00	///< Timer Stopped
#define TIMER_CLK_DIV1		0x01	///< Timer clocked at F_CPU
#define TIMER_CLK_DIV8		0x02	///< Timer clocked at F_CPU/8
#define TIMER_CLK_DIV64		0x03	///< Timer clocked at F_CPU/64
#define TIMER_CLK_DIV256	0x04	///< Timer clocked at F_CPU/256
#define TIMER_CLK_DIV1024	0x05	///< Timer clocked at F_CPU/1024

// This defines which bits within the TCCRnB register determine the prescale factor:
#define TIMER_PRESCALE_MASK	0xF8        // 0B11111000

// Map desired prescale divider to register bits:
byte timer_prescale_bits(int prescale) {
	if (prescale == 1)
		return 0x01;
	else if (prescale == 8)
		return 0x02;
	else if (prescale == 64)
		return 0x03;
	else if (prescale == 256)
		return 0x04;
	else if (prescale == 1024)
		return 0x05;
	else
		return 0x00;	// Error?!
}


// Function for computing the PWM wrap limit ("top" value) based on the desired frequency (and the CPU clock speed and prescaler setting):
// TODO: parameterise for prescale, rather than relying on it being globally defined?
unsigned int top(float frequency) {
	return round(F_CPU / (prescale * frequency) - 1);
}


void pwm(float frequency, float duty_cycle) {
	TCNT1 = 0;        // Reset timer counter
//	pwm_off(); // Maybe necessary to avoid stuck at 5 V condition? Nope, not enough...
	
	unsigned int wrap_limit = top(frequency);
	OCR1A = wrap_limit;
	OCR1B = int(wrap_limit * duty_cycle);

	// TCCR1A's timer mode should already be set, so just use TCCR1B to start PWM (is that sufficient?):
	TCCR1A = _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);	
	TCCR1B = (TCCR1B & TIMER_PRESCALE_MASK) | timer_prescale_bits(prescale);
}

void pwm_off() {
	TCCR1B = (TCCR1B & TIMER_PRESCALE_MASK) | TIMER_CLK_STOP;
	TCNT1 = 0;
	digitalWrite(OUTPUT_PIN, LOW); // This seems to be necessary to silence it properly (sometimes gets stuck high otherwise!)
}


void fan_speed(float target) {
	pwm(PWM_FREQUENCY, target);
}

void self_test() {
	fan_speed(0.1);
	delay(2000);
	fan_speed(0.5);
	delay(5000);
//	fan_speed(1.0);
//	delay(6000);
	fan_speed(0);
	delay(6000);
}

void note_on(int note, int velocity) {
	fan_speed(note / 127.0);	// Could also derive fan speed from note velocity
}

void note_off(int note, int velocity) {
	// Just leave fan running at current speed
}
