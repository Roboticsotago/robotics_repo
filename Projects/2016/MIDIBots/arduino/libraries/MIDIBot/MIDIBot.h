/*
 * Arduino library for our MIDIBot shield circuit board, Robocup 2016
 * Team _underscore_, Information Science Mechatronics, University of Otago
*/

// TODO:
// [ ] Have the use of Serial be generic?  Or maybe it's OK to hardcode to use the hardware serial always, since we're dealing with hardware shields.
// [ ] Convert to camelCase throughout for function names?  It's recommended in the Arduino API style guide.  Also member variables?
// [ ] Rename _i to something more meaningful, e.g. _dataByteIndex
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
	SELF_TEST_PIN = A5
;

//void note_on(int note, int velocity);
//void note_off(int note, int velocity);

class MIDIBot {
	public:
		MIDIBot();	// Class-specific setup(), essentially. Maybe pass Serial object?  Or hard-code to that internally?  Given hardware is hardwired, maybe OK to do that.
		void clearData();
		void flash(int on_time, int off_time);
		void flash_number(int n);
		void read_MIDI_channel();
		//void self_test();	// self_test() will be specific to each robot, but a reasonable basis would be to re-read the MIDI channel from the DIP switches and flash the number.
		void process_MIDI();	// Main MIDI-processing "loop"
	//	void note_off(int note, int velocity);	// Define note_on/off within the class and require user to subclass, or just assume externally defined?
	//	void note_on(int note, int velocity);
		// Generic self-test functions:
		void test_blink();
		void test_button();
		void test_MIDI_channel();
	private:
		int _MIDI_channel;	// 1..16, 10=drums
		
		// MIDI messages have up to 2 data bytes, which we store in an array:
		// TODO: perhaps name the index more sensibly/explicitly:
		int _dataByte[2], _statusByte, _i;
		
		// For monophonic instruments, it's useful to keep track of the note currently being played, so that any note-off messages for other notes can be ignored.
		int _current_note_number;
};

//#include "MIDIBot.cpp"

#endif
