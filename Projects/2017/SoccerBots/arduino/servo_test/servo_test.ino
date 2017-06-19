// Emergency PopperBot code! FanBot problems!

#include <Servo.h> 
#include <MIDIBot.h>
//#include "/home/pi/robotics_repo/Projects/2016/MIDIBots/arduino/libraries/MIDIBot/MIDIBot.h"
MIDIBot popperBot;

Servo partyPopperServo;

const int PARTY_POPPER_MAX = 50; 
const int PARTY_POPPER_MIN = 140;
const int PARTY_POPPER_MID = 95;

// MIDI note mapping:
const int PARTY_POPPER_FIRE_NOTE =  3;
const int PART_POPPER_RELEASE_NOTE= 4;


void setup() {
	popperBot.begin();

	partyPopperServo.attach(SERVO_3_PIN);
	partyPopperServo.write(PARTY_POPPER_MIN);
}

void loop() {
	popperBot.process_MIDI();
}

void self_test() {
	party_popper_fire();
}

void party_popper_fire() {
	partyPopperServo.write(PARTY_POPPER_MIN);
	digitalWrite(LED_PIN, HIGH);
	delay(1000);
	digitalWrite(LED_PIN, LOW);
	partyPopperServo.write(PARTY_POPPER_MAX);
	delay(1000);
	partyPopperServo.write(PARTY_POPPER_MID);
} 


void note_on(int note, int velocity) {
	switch (note) {
			case PARTY_POPPER_FIRE_NOTE: party_popper_fire(); break;
	}
}

void note_off(int note, int velocity) {
}
