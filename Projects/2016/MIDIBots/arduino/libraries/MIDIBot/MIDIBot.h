/*

Arduino library for our MIDIBot shield circuit board, Robocup 2016
Team _underscore_, Information Science Mechatronics, University of Otago

To use this library, #include it, and instantiate a la "MIDIBot drumBot;" (note: no trailing parentheses - that would make it a function declaration!).

You will also need to define a note_on() and note_off() implementation for the specific robot. These will be called from MIDIBot::process_MIDI, passing the MIDI note number and velocity as arguments.

Your sketch will also need to implement a self_test() function, which will be called from MIDIBot::process_MIDI if the Self-Test button is pressed.

Because you can't use delay() or Serial.begin() in the constructor, this class defines a separate MIDIBot::begin() method which reads the MIDI channel from the DIP switches, flashes the MIDI channel number, and starts serial communication at MIDI baud rate (after a 5-second pause to avoid reprogramming problems).

The following is a basic example:
--
#include <MIDIBot.h>
MIDIBot thisMIDIBot;

void note_on(int note, int velocity) {
	digitalWrite(LED_PIN, HIGH);
	analogWrite(MOSFET_PWM_PIN, 64);
}

void note_off(int note, int velocity) {
	digitalWrite(LED_PIN, LOW);
	analogWrite(MOSFET_PWM_PIN, 0);
}

void self_test() {
	thisMIDIBot.test_MIDI_channel();
	// Bot-specific tests...
}

void setup() {
	thisMIDIBot.test_MIDI_channel();	// Indicate MIDI channel at startup
}

void loop() {
	thisMIDIBot.process_MIDI();
}
--
*/


// NOTE:
//  - Due to apparent problems with calling delay() inside a constructor, you can't flash the MIDI channel number on the LED automatically at startup (though you can read the MIDI channel DIP switches).  Therefore, call yourMIDIBot.test_MIDI_channel() it your setup() function if you want this to happen.

// TODO:
// [ ] Have the use of Serial be generic?  Or maybe it's OK to hardcode to use the hardware serial always, since we're dealing with hardware shields.
// [ ] Convert to camelCase throughout for function names?  It's recommended in the Arduino API style guide.  Also member variables?
// [ ] Rename _i to something more meaningful, e.g. _dataByteIndex or _MIDIBufferIndex
// [ ] #define constants?  That seems to be the norm with .h files, and they'd only be defined if the header were included, and might save on processor registers?

#ifndef MIDIBot_included
#define MIDIBot_included

#include "Arduino.h"


// Define pin mappings for the MIDIBot shield:
// For a library, should these be global symbols or defined within the class?
// Also, how do we make the class a singleton?

// Should these be consts or #defines?
const int
	MIDI_1x_PIN = 2,
	MIDI_2x_PIN = 3,
	MIDI_4x_PIN = 4,	/* USBDroid has pulldown resistor on D4! */
	MIDI_8x_PIN = 5,
	MOSFET_PWM_PIN = 6,
	SERVO_1_PIN = 9,	/* USBDroid has pulldown resistor on D9! */
	SERVO_2_PIN = 10,
	SERVO_3_PIN = 11,	/* USBDroid has pulldown resistor on D11! */
	MOSFET_2_PIN = 12,
	MOSFET_3_PIN = A1,	/* not A3 as on the silkscreen! */
	MOSFET_4_PIN = A2,	/* not A4 as on the silkscreen! */
	LED_PIN = 13,		/* USBDroid has pulldown resistor on D13! */
	LATENCY_ADJUST_PIN = A0,
	SELF_TEST_PIN = A5,
	SERIAL_SPEED = 31250
	
;

// Empty prototypes for additional functions that the sketch using this library should define (bot-specific functionality):
// (Do these need to be declared "extern" as well?)
void note_on(int note, int velocity);
void note_off(int note, int velocity);
void self_test(); // Called from process_MIDI() when the self-test button is pressed


class MIDIBot {
	public:
		MIDIBot();	// Constructor method.  Sets up the MIDIBot Shield's pins, sets the MIDI channel ID from the DIP switches, and starts Serial input for MIDI compatibility.
		void begin();	// Start serial comms
		void clearData();	// Zeroes out the buffer for received MIDI data
		void flash(int on_time, int off_time);	// Flash the onboard LED with specified on/off duration in milliseconds
		void flash_number(int n);	// Flash the LED in a pattern to indicate the value of <n>
		void read_MIDI_channel();	// Read MIDI DIP switch settings and store in _MIDI_channel
		void process_MIDI();	// Main MIDI-processing "loop"
		// Generic self-test functions:
		void test_blink();	// Blink the onboard LED once
		void test_button();	// Light the onboard LED if the self-test button is pressed
		void test_MIDI_channel();	// Read and flash-display the MIDI channel number
	private:
		int _MIDI_channel;	// 1..16, 10=drums
		
		// MIDI messages have up to 2 data bytes, which we store in an array:
		// TODO: perhaps name the index more sensibly/explicitly:
		int _dataByte[2], _statusByte, _i;
		
		// For monophonic instruments, it's useful to keep track of the note currently being played, so that any note-off messages for other notes can be ignored.
		int _current_note_number;
};

#endif
