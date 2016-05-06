// MIDI note tests for Mechatronics, CME 2016-03-21
// 1. Loop sanity check.  Watch for off-by-one errors!
// 2. Translate note frequency formula into C syntax and check (MIDI note 69 should give 440 Hz). 
// 3. Correct integer division in formula!
// 4. Derive OCR "top" value from frequency, and print for checking.
// 5. Correct type of "top" to get the full available range.
// 6. Move the formulae into separate functions, and check that it still works!
// 7. Add stuff for generating the PWM output.  Tweak range of notes for musical range.


// General settings:

const int prescale = 8;
const int serial_speed = 9600;

// On the Mega, we have timer1 attached to pins D11 and D12, D12 being the primary one.
// On "ordinary" Arduinos, it's on pins 9 and 10.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
const int OUTPUT_PIN     = 12,
const int OUTPUT_PIN_AUX = 11;
#else
const int OUTPUT_PIN     = 10;
const int OUTPUT_PIN_AUX = 9;
#endif

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


float frequency(int note) {
	return 440 * pow(2, (note - 69) / 12.0);
}

unsigned int top(float frequency) {
	return round(F_CPU / (prescale * frequency) - 1);
}


void pwm(float frequency, float duty_cycle) {
	TCNT1 = 0;        // Reset timer counter
	
	unsigned int wrap_limit = top(frequency);
	OCR1A = wrap_limit;
	OCR1B = int(wrap_limit * duty_cycle);

	// TCCR1A's timer mode should already be set, so just use TCCR1B to start PWM (is that sufficient?):
	TCCR1A = _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);	
	TCCR1B = (TCCR1B & TIMER_PRESCALE_MASK) | timer_prescale_bits(prescale);
}
	
void setup() {
	pinMode(OUTPUT_PIN, OUTPUT);
	pinMode(OUTPUT_PIN_AUX, OUTPUT);
	digitalWrite(OUTPUT_PIN, LOW);
	digitalWrite(OUTPUT_PIN_AUX, LOW);
	delay(8000);  // safety delay for serial I/O
	Serial.begin(serial_speed);
}

void loop() {
	int prescale = 8;
	for (int note = 33; note <= 96; note++) {
		float f = frequency(note);
		unsigned int t = top(f);
		Serial.print(note);
		Serial.print(": f=");
		Serial.print(f);
		Serial.print(", top=");
		Serial.println(t);
		pwm(frequency(note), 0.3);
		delay(150);
	}
}
