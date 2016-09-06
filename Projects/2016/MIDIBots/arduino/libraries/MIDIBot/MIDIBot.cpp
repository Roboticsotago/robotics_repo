/*
 * Arduino library for our MIDIBot shield circuit board, Robocup 2016
 * Team _underscore_, Information Science Mechatronics, University of Otago
*/

#include "Arduino.h"
#include "MIDIBot.h"

// Constructor method:
// This performs most of the low-level setup.  Much of this will be standard, but some firmware-specific setup code will also be likely.  That can be done in the usual setup() in the main sketch.
// Some setup functions cannot be done in the constructor (notably Serial.begin() and anything that uses delay()).  Thus there is a separate begin() method that should be called from the main sketch during setup().
// self_test() will need to be implemented by the sketch using this library.
// Ditto note_on(), note_off().

MIDIBot::MIDIBot()
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
	
	// Initialise MIDI channel number according to DIP switch settings:
	read_MIDI_channel();
//	flash_number(_MIDI_channel + 1);	// This seems to be the cause of the problems with using this as a library!  I suspect it's because it uses delay() internally.
	
	// NOTE: MIDI serial communication must be set up elsewhere, in the ::begin() method.  If it's done here, the program won't hang, but serial communication will not work!
	// Also note that use of the delay() function in this constructor will cause the program to hang!

	clearData();
}

void MIDIBot::begin(){
	delay(250);
	test_MIDI_channel();
	delay(1500);
	// self_test();
	flash(100, 100);
	Serial.begin(SERIAL_SPEED);
}

// erase the MIDI data buffer
void MIDIBot::clearData(){
	_dataByte[0] = 0;
	_dataByte[1] = 0;
	_i = 0;
}


void MIDIBot::flash(int on_time, int off_time) {
	digitalWrite(LED_PIN, HIGH);
	delay(on_time);
	digitalWrite(LED_PIN, LOW);
	delay(off_time);	
}


// Routine for displaying a number by blinking the LED (since hardware serial will be taken already by the MIDI comms):
void MIDIBot::flash_number(int n) {
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
// NOTE: Freetronics USBDroid has a pull-down resistor on pin D4, which makes that pin unusable! It acts as if D4 is always grounded, i.e. the switch is on.  Multiply that bit by 0 (instead of 4) here to force it off.
void MIDIBot::read_MIDI_channel() {
	_MIDI_channel =
		!digitalRead(MIDI_8x_PIN) * 8 +
		!digitalRead(MIDI_4x_PIN) * 4 +
		!digitalRead(MIDI_2x_PIN) * 2 +
		!digitalRead(MIDI_1x_PIN);
}


// The process_MIDI() function performs receiving incoming MIDI data via serial, recognising note-on and note-off messages (and, perhaps later on, things like Control Change messages), and calls the note_on(pitch, velocity), note_off(pitch, velocity) functions defined by the calling sketch.  It should be called repeatedly by the loop() function in the main sketch.
// It also handles calling the self_test() function (defined by the sketch using this library) if the self-test button is pressed.  That's done here so that no extra code is required in the calling sketch for this to work.
// TODO: Perhaps define constants, such as NOTE_ON=0x90 (or 0x9)?
// TODO: copy _dataByte[0..1] to meaningfully-named variables (pitch/note, velocity) for better readability? Though it's not really necessary, and possibly inefficient.
void MIDIBot::process_MIDI() {
	
	if (!digitalRead(SELF_TEST_PIN)) {
		self_test();
	}

	if (Serial.available() > 0) {
		// flash(20,0); // Consider enabling this for debugging. Warning: blocks!
		int data = Serial.read();
		if (data > 127) {
			// It's a status byte. Store it for future reference.
			_statusByte = data;
			clearData();  
		} else {
			// It's a data byte.
			_dataByte[_i] = data;
			if (_statusByte == (0x90 | _MIDI_channel) && _i == 1) {
				// Note-on message received
				if (_dataByte[1] == 0) {
					// Note-on with velocity=0: equialent to note-off, so stop note playing (nothing to do for percussion!)
					note_off(_dataByte[0], _dataByte[1]);
				} else {
					// Start note playing
					note_on(_dataByte[0], _dataByte[1]);
				}
			} else if (_statusByte == (0x80 | _MIDI_channel) && _i == 1) {
				// Note-off message received
				note_off(_dataByte[0], _dataByte[1]);
			}
			_i++;
			// TODO: error detection if _i goes beyond the array size.
		}
	}
}


// Some standard self-test routines:

void MIDIBot::test_blink() {
	digitalWrite(LED_PIN, HIGH);
	delay(500);
	digitalWrite(LED_PIN, LOW);
	delay(500);
}


void MIDIBot::test_button() {
	digitalWrite(LED_PIN, !digitalRead(SELF_TEST_PIN));
}


void MIDIBot::test_MIDI_channel() {
	read_MIDI_channel();
	flash_number(_MIDI_channel + 1);
}


void test_MOSFETs() {
	if (!digitalRead(SELF_TEST_PIN)) {
		analogWrite(MOSFET_PWM_PIN, 64);
		digitalWrite(MOSFET_2_PIN, HIGH);
		digitalWrite(MOSFET_3_PIN, HIGH);
		digitalWrite(MOSFET_4_PIN, HIGH);
	} else {
		analogWrite(MOSFET_PWM_PIN, 0);
		digitalWrite(MOSFET_2_PIN, LOW);
		digitalWrite(MOSFET_3_PIN, LOW);
		digitalWrite(MOSFET_4_PIN, LOW);
	}
}


void test_MOSFETs_cycle() {
	analogWrite(MOSFET_PWM_PIN, 64);
	delay(250);
	analogWrite(MOSFET_PWM_PIN, 0);
	
	digitalWrite(MOSFET_2_PIN, HIGH);
	delay(250);
	digitalWrite(MOSFET_2_PIN, LOW);
	
	digitalWrite(MOSFET_3_PIN, HIGH);
	delay(250);
	digitalWrite(MOSFET_3_PIN, LOW);

	digitalWrite(MOSFET_4_PIN, HIGH);
	delay(250);
	digitalWrite(MOSFET_4_PIN, LOW);
}


void test_PWM() {
	analogWrite(MOSFET_PWM_PIN, 64);
	delay(250);
	analogWrite(MOSFET_PWM_PIN, 0);
	delay(500);
}
