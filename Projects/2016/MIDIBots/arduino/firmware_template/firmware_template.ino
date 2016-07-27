// ____Bot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

#include <MIDIBot.h>
MIDIBot thisMIDIBot;

void note_on(int note, int velocity) {
	// TODO...
}

void note_off(int note, int velocity) {
	// TODO...
}

void self_test() {
//	thisMIDIBot.test_MIDI_channel();
	// TODO: Bot-specific self-test routine...
}

void setup() {
	thisMIDIBot.test_MIDI_channel();	// Indicate MIDI channel at startup
}

void loop() {
	if (!digitalRead(SELF_TEST_PIN)) {
		self_test();
	}
	thisMIDIBot.process_MIDI();
}
