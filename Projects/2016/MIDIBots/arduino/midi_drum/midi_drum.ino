// An Arduino program for triggering a servo-controlled drum-stick in response to MIDI note messages


// General settings:
const float DOWN = 0;
const float UP = 0.5;
 
const int prescale = 8;
const int SERVO_FREQUENCY = 50; // Hz (1/(20 ms))
const int channel = 0;


// MIDI messages have up to 2 data bytes, which we store in an array:
int dataByte[2], statusByte = 0, i = 0;

void clearData(){
  dataByte[0] = 0;
  dataByte[1] = 0;
  i = 0;
}

// Even though this is only monophonic, it's still useful to keep track of the note currently being played, so that any note-off messages for other notes can be ignored.
int current_note_number = 0;

// On the Mega, we have timer1 attached to pins D11 and D12, D12 being the primary one.
// On "ordinary" Arduinos, it's on pins 9 and 10.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
const int OUTPUT_PIN     = 12;
const int OUTPUT_PIN_AUX = 11;
#else
const int OUTPUT_PIN     = 10;
const int OUTPUT_PIN_AUX = 9;
#endif

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

void pwm_off() {
    return;
	TCCR1B = (TCCR1B & TIMER_PRESCALE_MASK) | TIMER_CLK_STOP;
	TCNT1 = 0;
	digitalWrite(OUTPUT_PIN, LOW); // This seems to be necessary to silence it properly (sometimes gets stuck at 5 V otherwise!)
}
	
void setup() {
	pinMode(OUTPUT_PIN, OUTPUT);
	pinMode(OUTPUT_PIN_AUX, OUTPUT);
	digitalWrite(OUTPUT_PIN, LOW);
	digitalWrite(OUTPUT_PIN_AUX, LOW);
	clearData();
        pwm(50, calcDutyCicle(UP));
	delay(2000);  // safety delay for serial I/O
	Serial.begin(31250);
        
}

float calcDutyCicle(float in){
      if(in < 0)
              in = 0;
      if(in > 1)
              in = 1;
      return (in + 1)/20.0;
}

void drum(){
        int spd = 100; // ms; 80 ms is OK up to about 220 BPM
        pwm(SERVO_FREQUENCY, calcDutyCicle(DOWN));
        delay(spd);
        pwm(SERVO_FREQUENCY, calcDutyCicle(UP));
        delay(spd);
}

void loop  () {
	if (Serial.available() > 0) {
		int data = Serial.read();
		if (data > 127) {
			// It's a status byte. Store it for future reference.
			statusByte = data;
			clearData();  
		} else {
			// It's a data byte.
			dataByte[i] = data;
			if (statusByte == (0x90 | channel) && i == 1) {
				// Note-on message received
				if (dataByte[1] == 0 && dataByte[0] == current_note_number) {
					// Stop note playing
					pwm_off();
				} else {
					// Start note playing
					current_note_number = dataByte[0];
//					pwm(frequency(current_note_number), dataByte[1] / 127.0 / 2); // TODO: map velocity to PWM duty
				        drum();
                                }
			} else if (statusByte == (0x80 | channel) && i == 1 && dataByte[0] == current_note_number) {
				// Note-off message received
				// Stop note playing
				pwm_off();
			}
			i++;
			// TODO: error detection if i goes beyond the array size.
		}
	}
}

