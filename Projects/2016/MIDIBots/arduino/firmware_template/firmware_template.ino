// Template/skeleton code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

#include <MIDIBot.h>

// Additional functions required to be defined by the MIDIBot library:

void note_on(int note, int velocity) {
	digitalWrite(LED_PIN, HIGH);
	analogWrite(MOSFET_PWM_PIN, 64);
}

void note_off(int note, int velocity) {
	digitalWrite(LED_PIN, LOW);
	analogWrite(MOSFET_PWM_PIN, 0);
}


// Instantiate the object from the library:

MIDIBot testMIDIBot;

// NOTE: you have to define self_test() after the library has been instantiated, otherwise the library object's functions won't be available.
void self_test() {
	// Robot-specific self-test routine goes here. Example:
	if (!digitalRead(SELF_TEST_PIN)) {
		testMIDIBot.test_MIDI_channel();
	}
}

void setup() {
	testMIDIBot.test_MIDI_channel();
}

void loop() {
	testMIDIBot.test_button();
//  self_test();
//  testMIDIBot.test_blink();
}
