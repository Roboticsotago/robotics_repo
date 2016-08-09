// SynthBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago
// NOTE: there's a limit to how low a frequency this can play, given the PWM settings.  Note frequencies will jump around below this limit.

/*
 * TODO:
 * [ ] Fix note-playing glitch at startup (first note is a weird bleep)
 * [ ] Have MIDI velocity mapped to PWM duty cycle.
*/

int current_note_number = 0;
#include <MIDIBot.h>
MIDIBot synthBot;

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

// Map MIDI note numbers to frequency in Hz:
float frequency(int note) {
	return 440 * pow(2.0, (note - 69) / 12.0);
}

// Function for computing the PWM wrap limit ("top" value) based on the desired frequency (and the CPU clock speed and prescaler setting):
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
	digitalWrite(OUTPUT_PIN, LOW); // This seems to be necessary to silence it properly (sometimes gets stuck at 5 V otherwise!)
}
	

void self_test() {
	// Robot-specific self-test routine goes here
	pwm(frequency(40), 0.1); delay(250);
	pwm(frequency(52), 0.1); delay(250);
	pwm(frequency(64), 0.1); delay(250);
	pwm(frequency(76), 0.1); delay(250);
	pwm_off();
}

void setup()
{
	synthBot.begin();
	pwm_off();
}

void loop()
{
	synthBot.process_MIDI();
}

void note_on(int note, int velocity){
	// Start note playing
	current_note_number = note;	// Store current note for future reference (since we're monophonic)
	// We map MIDI velocity to PWM duty cycle.  Divide by two since the upper half (1..0.5) sounds the same as the lower half (0..0.5).
	pwm(frequency(current_note_number), velocity / 127.0 / 2.0);
	// Plus illumination:
	digitalWrite(LED_PIN, HIGH);
	analogWrite(MOSFET_PWM_PIN, 64);	
}

void note_off(int note, int velocity){
	// Because this is monophonic, we only want to turn the note off if the note-off message we just received is for the note we're currently playing.
	// That is, ignore any note-offs for notes other than the current one.
	if (note == current_note_number) {
		pwm_off();
		digitalWrite(LED_PIN, LOW);
		analogWrite(MOSFET_PWM_PIN, 0);
	}
}
