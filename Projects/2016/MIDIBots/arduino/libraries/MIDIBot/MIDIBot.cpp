/*
 * Arduino library for our MIDIBot shield circuit board, Robocup 2016
 * Team _underscore_, Information Science Mechatronics, University of Otago
*/

#include "Arduino.h"
#include "MIDIBot.h"

// Constructor:
// At least some of the setup routine will be standard, but some firmware-specific setup code will also be likely.  That can be done in the usual setup().
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
	
	// Set up MIDI communication:
	// TODO: might be a good idea to introduce a delay here before opening Serial to make reprogramming a bit less unreliable.
//	delay(6000);	// No, that also causes things to fail!  Maybe anything calling delay() in the constructor will cause the sketch to hang.

	// NOTE: Do not call Serial.begin() in the constructor either! It doesn't hang, but serial communication will not work subsequently.
	//Serial.begin(31250);
//	Serial.begin(9600);

	clearData();
	
	// Attach interrupt for self-test button and function:
//	attachInterrupt(digitalPinToInterrupt(SELF_TEST_PIN), self_test, FALLING);
	// Only available on certain pins?  Just poll in main loop instead.
	
	// Flash to indicate startup/ready:
//	flash(50, 200); flash(50, 400); flash(50, 200); flash(400, 0);
	// Don't bother if flashing MIDI channel - it's confusing!

}

void MIDIBot::begin(){
	delay(5000);
	Serial.begin(SERIAL_SPEED);
	test_MIDI_channel();
}

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
// NOTE: Freetronics USBDroid has a pull-down resistor on pin D4, which makes that pin unusable! It acts as if D4 is always grounded, i.e. the switch is on.
// Multiply the 4x bit by 4 on a normal Arduino, or 0 to force it off.
void MIDIBot::read_MIDI_channel() {
	_MIDI_channel =
		!digitalRead(MIDI_8x_PIN) * 8 +
		!digitalRead(MIDI_4x_PIN) * 4 +
		!digitalRead(MIDI_2x_PIN) * 2 +
		!digitalRead(MIDI_1x_PIN);
}


// self_test() will need to be implemented by the sketch using this library.
// Ditto note_on(), note_off().


// Can we factor out some parts of process_MIDI into common functions, and make process_MIDI generic?  Perhaps define constants, such as NOTE_ON=0x90?
// We could have it expect functions note_on(pitch, velocity), note_off(pitch, velocity), but I think these will need prototype/forward declarations.
// TODO: copy _dataByte[0..1] to meaningfully-named variables (pitch/note, velocity) for better readability?
void MIDIBot::process_MIDI() {
	if (!digitalRead(SELF_TEST_PIN)) {
		self_test();
	}
	if (Serial.available() > 0) {
		flash(20,0); // Warning: blocks!
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
					// Note-on with velocity=0: Stop note playing (nothing to do for percussion!)
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
/*
	Serial.print(_MIDI_channel);
	Serial.print(" (");
	Serial.print(_MIDI_channel + 1);
	Serial.println(")");
*/
}
