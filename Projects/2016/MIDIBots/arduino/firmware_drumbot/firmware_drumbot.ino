// DrumBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

#include <MIDIBot.h>
MIDIBot drumBot;

#include "Timer.h"
#include <Servo.h> 

Servo
	bass_drum_servo,
	snare_drum_servo,
	cymbal_servo
;

const int BASS_DRUM_NOTE = 36;
const int BASS_DRUM_MIN = 30;
const int BASS_DRUM_MAX = 12;
const int BASS_DRUM_DELAY = 100;

const int SNARE_DRUM_NOTE = 39;
const int SNARE_DRUM_MIN = 40;
const int SNARE_DRUM_MAX = 62;
const int SNARE_DRUM_DELAY = 100;

const int CYMBAL_NOTE = 42;
const int CYMBAL_MIN = 60;
const int CYMBAL_MAX = 90;	
const int CYMBAL_DELAY = 100;


// Timers for asynchronous release of drum hits:

Timer *bass_drum_timer = new Timer(BASS_DRUM_DELAY, &bass_drum_release, 1);
Timer *snare_drum_timer = new Timer(SNARE_DRUM_DELAY, &snare_drum_release, 1);
Timer *cymbal_timer = new Timer(CYMBAL_DELAY, &cymbal_release, 1);

// Separate functions for hit and release for each drum:

void bass_drum_hit() {
	bass_drum_servo.write(BASS_DRUM_MAX);
	bass_drum_timer->Start();
}
void snare_drum_hit() {
	snare_drum_servo.write(SNARE_DRUM_MAX);
	snare_drum_timer->Start();
}
void cymbal_hit() {
	cymbal_servo.write(CYMBAL_MAX);
	cymbal_timer->Start();
}

void bass_drum_release() {bass_drum_servo.write(BASS_DRUM_MIN);}
void snare_drum_release() {snare_drum_servo.write(SNARE_DRUM_MIN);}
void cymbal_release() {cymbal_servo.write(CYMBAL_MIN);}


void self_test() {
	bass_drum_hit();
	snare_drum_hit();
	cymbal_hit();
}

void note_on(int note, int velocity) {
	switch (note) {
		case BASS_DRUM_NOTE:  bass_drum_hit();  break;
		case CYMBAL_NOTE:     cymbal_hit();     break;
		case SNARE_DRUM_NOTE: snare_drum_hit(); break;
	}
}

void note_off(int note, int velocity) {
	//nothing for drumBot.
}

void setup()
{
	// Servo setup:
	bass_drum_servo.attach(SERVO_1_PIN);
	snare_drum_servo.attach(SERVO_2_PIN);
	cymbal_servo.attach(SERVO_3_PIN);

	// Move motors to starting positions
	bass_drum_servo.write(BASS_DRUM_MIN);
	snare_drum_servo.write(SNARE_DRUM_MIN);
	cymbal_servo.write(CYMBAL_MIN);

	drumBot.begin();
}

void loop()
{
	bass_drum_timer->Update();
	snare_drum_timer->Update();
	cymbal_timer->Update();
	
	drumBot.process_MIDI();
}
