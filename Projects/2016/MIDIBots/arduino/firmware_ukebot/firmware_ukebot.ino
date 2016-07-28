// UkeBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago


// Define pin mappings for the MIDIBot shield:
const int
	MIDI_1x_PIN = 2,
	MIDI_2x_PIN = 3,
	MIDI_4x_PIN = 4,
	MIDI_8x_PIN = 5,
	MOSFET_PWM_PIN = 6,
	SERVO_1_PIN = 9,	/* Pin 9 is misbehaving! */
	SERVO_2_PIN = 10,
	SERVO_3_PIN = 11,
	MOSFET_2_PIN = 12,
	MOSFET_3_PIN = A1, /* not A3 as on the silkscreen! */
	MOSFET_4_PIN = A2, /* not A4 as on the silkscreen! */
	LED_PIN = 13,
	LATENCY_ADJUST_PIN = A0,
	SELF_TEST_PIN = A5
;


#include <Servo.h> 

Servo strum;
Servo slide;

int pos = 0;    // variable to store the servo position

const int UP_NOTE = 60; // Middle C
const int DOWN_NOTE = 62; // D

const int STRUM_MIN = 90;
const int STRUM_MAX = 110;

const int SLIDE_MIN = 30;
const int SLIDE_MAX = 100;

// Unused?
const int STRUM_DELAY = 100;

void down_strum() {
	strum.write(STRUM_MAX);
}

void up_strum() {
	strum.write(STRUM_MIN);
}

void down_slide() {
	slide.write(SLIDE_MAX);
}

void up_slide() {
	slide.write(SLIDE_MIN);
}



int MIDI_channel = 0;	// 1..16, 10=drums

// MIDI messages have up to 2 data bytes, which we store in an array:
int dataByte[2], statusByte = 0, i = 0;

void clearData(){
  dataByte[0] = 0;
  dataByte[1] = 0;
  i = 0;
}

// For monophonic instruments, it's useful to keep track of the note currently being played, so that any note-off messages for other notes can be ignored.
int current_note_number = 0;


void flash(int on_time, int off_time) {
	digitalWrite(LED_PIN, HIGH);
	delay(on_time);
	digitalWrite(LED_PIN, LOW);
	delay(off_time);	
}

// Routine for displaying a number by blinking the LED (since hardware serial will be taken already by the MIDI comms):
void flash_number(int n) {
	delay(500);
	// Hundreds:
	for (int i = 0; i < (n % 1000 / 100); i++) {
		flash(800, 800);
	}
	delay(500);
	// Tens:
	for (int i = 0; i < (n % 100 / 10); i++) {
		flash(400, 400);
	}
	delay(500);
	// Fastest flashing for the ones:
	for (int i = 0; i < (n % 10); i++) {
		flash(200, 200);
	}
	delay(1000);
}


// Read MIDI channel DIP switches and store the result:
// NOTE: Freetronics USBDroid has a pull-down resistor on pin D4, which makes that pin unusable! It acts as if D4 is always grounded, i.e. the switch is on.
// Multiply the 4x bit by 4 on a normal Arduino, or 0 to force it off.
void read_MIDI_channel() {
	MIDI_channel =
		!digitalRead(MIDI_8x_PIN) * 8 +
		!digitalRead(MIDI_4x_PIN) * 4 +
		!digitalRead(MIDI_2x_PIN) * 2 +
		!digitalRead(MIDI_1x_PIN);
}

void self_test() {
	// Robot-specific self-test routine goes here
//	read_MIDI_channel();
//	flash_number(MIDI_channel + 1);
	down_strum();
	down_slide();
	delay(500);
	up_slide();
	up_strum();
}

void setup()
{
	// Set up pin modes for the MIDIBot Shield (input, internal pull-up resistor enabled):
	pinMode(MIDI_1x_PIN, INPUT_PULLUP);
	pinMode(MIDI_2x_PIN, INPUT_PULLUP);
	pinMode(MIDI_4x_PIN, INPUT_PULLUP);
	pinMode(MIDI_8x_PIN, INPUT_PULLUP);
	
	// Ditto for self-test button:
	pinMode(SELF_TEST_PIN, INPUT_PULLUP);
	
	// TODO: output pins (servos, MOSFETs, LED)
	pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, LOW);
	pinMode(MOSFET_PWM_PIN, OUTPUT); analogWrite(MOSFET_PWM_PIN, 0);
	pinMode(MOSFET_2_PIN, OUTPUT); digitalWrite(MOSFET_2_PIN, LOW);
	pinMode(MOSFET_3_PIN, OUTPUT); digitalWrite(MOSFET_3_PIN, LOW);
	pinMode(MOSFET_4_PIN, OUTPUT); digitalWrite(MOSFET_4_PIN, LOW);	
	pinMode(SERVO_1_PIN, OUTPUT); digitalWrite(SERVO_1_PIN, LOW);
	pinMode(SERVO_2_PIN, OUTPUT); digitalWrite(SERVO_2_PIN, LOW);
	pinMode(SERVO_3_PIN, OUTPUT); digitalWrite(SERVO_3_PIN, LOW);
	
	// Servo setup:
	strum.attach(SERVO_2_PIN);
	slide.attach(SERVO_3_PIN);
	
	// Set initial position
	strum.write(STRUM_MIN);
	slide.write(STRUM_MIN);

	// Initialise MIDI channel number according to DIP switch settings:
	read_MIDI_channel();
	flash_number(MIDI_channel);
	
	// Set up MIDI communication:
	Serial.begin(31250);
	clearData();
	
	// Attach interrupt for self-test button and function:
//	attachInterrupt(digitalPinToInterrupt(SELF_TEST_PIN), self_test, FALLING);
	// Only available on certain pins?  Just poll in main loop instead.
	
	// Flash to indicate startup/ready:
//	flash(50, 200); flash(50, 400); flash(50, 200); flash(400, 0);
	// Don't bother if flashing MIDI channel - it's confusing!
}

void loop()
{
	if (!digitalRead(SELF_TEST_PIN)) {
		self_test();
	}
		down_slide();
		delay(500);
		up_strum();
		delay(500);
		down_strum();
		delay(500);
		
		up_slide();
		delay(500);
		up_strum();
		delay(500);
		down_strum();
		delay(500);
		
		
	//process_MIDI();
//	test_blink();
//	test_button();
//	test_flash_number();
//	test_MIDI_channel();
//	test_MOSFETs();
//	test_MOSFETs_cycle();
//	test_PWM();
//	test_servo();
}


void process_MIDI() {
	if (Serial.available() > 0) {
		int data = Serial.read();
		if (data > 127) {
			// It's a status byte. Store it for future reference.
			statusByte = data;
			clearData();  
		} else {
			// It's a data byte.
			dataByte[i] = data;
			if (statusByte == (0x90 | MIDI_channel) && i == 1) {
				// Note-on message received
		flash(20,0); // Warning: blocks!
				if (dataByte[1] == 0) {
					// Stop note playing - nothing to do!
				} else {
					// Start note playing
					if (dataByte[0] == DOWN_NOTE) {down_strum();}
					if (dataByte[0] == UP_NOTE) {up_strum();}
				}
			} else if (statusByte == (0x80 | MIDI_channel) && i == 1) {
				// Note-off message received
				// Nothing to do!
			}
			i++;
			// TODO: error detection if i goes beyond the array size.
		}
	}
}

void test_blink() {
	digitalWrite(LED_PIN, HIGH);
	delay(500);
	digitalWrite(LED_PIN, LOW);
	delay(500);
}

void test_button() {
	digitalWrite(LED_PIN, !digitalRead(SELF_TEST_PIN));
}

void test_MIDI_channel() {
	if (!digitalRead(SELF_TEST_PIN)) {
		read_MIDI_channel();
		Serial.print(MIDI_channel);
		Serial.print(" (");
		Serial.print(MIDI_channel + 1);
		Serial.println(")");
		flash_number(MIDI_channel + 1);
	}
}

void test_servo() {
	strum.write(60);
	delay(500);
	strum.write(110);
	delay(
	500);
}