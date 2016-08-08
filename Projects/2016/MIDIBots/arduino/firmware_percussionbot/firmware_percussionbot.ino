// PercussionBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

// TODO: add a DC motor operating a football/cog rattle? And a similar mechanism for repeatedly striking the triangle?

#include <MIDIBot.h>
MIDIBot percussionBot;

#include "Timer.h"
#include <Servo.h> 

Servo
	triangle_servo,
	shaker_servo,
	drum_servo
;

// TODO: Unused? Remove if so.
int pos = 0;    // variable to store the servo position

// TODO: carefully test required delay for each instrument and tweak *_DELAY constants below.

const int TRIANGLE_NOTE = 60; // Middle C
const int TRIANGLE_MIN = 30;
const int TRIANGLE_MAX = 14;
const int TRIANGLE_DELAY = 100;
// Checked!

const int SHAKER_NOTE = 39; //62
const int SHAKER_MIN = 100;
const int SHAKER_MAX = 110;
const int SHAKER_DELAY = 100;
// Checked! 

// TODO: (re)move these:
// Numbers here for DrumBot from testing:

// cymbal: 60..90 degrees, 100 ms
// snare drum: 40..62 degrees, 100 ms
// bass drum: 30..12, 100 ms

const int DRUM_NOTE = 36; //64
const int DRUM_MIN = 160;
const int DRUM_MAX = 137;	
const int DRUM_DELAY = 100;
// Checked!


// Timers for asynchronous release of drum hits:
Timer *triangle_timer = new Timer(TRIANGLE_DELAY, &triangle_release, 1);
Timer *shaker_timer = new Timer(SHAKER_DELAY, &shaker_release, 1);
Timer *drum_timer = new Timer(DRUM_DELAY, &drum_release, 1);


// Separate functions for hit and release for each drum:
void triangle_hit() {
	triangle_servo.write(TRIANGLE_MAX);
	triangle_timer->Start();
}
void shaker_hit() {
	shaker_servo.write(SHAKER_MAX);
	shaker_timer->Start();
}
void drum_hit() {
	drum_servo.write(DRUM_MAX);
	drum_timer->Start();
}

void triangle_release() {triangle_servo.write(TRIANGLE_MIN);}
void   shaker_release() {  shaker_servo.write(SHAKER_MIN);}
void     drum_release() {   drum_servo.write(DRUM_MIN);}


/*
// TODO: convert to switch statement instead?  See (untested) example code below...
void note_on(int note, int velocity) {
	if (note == TRIANGLE_NOTE) {
		triangle_hit();
	}
	if (note == SHAKER_NOTE) {
		shaker_hit();
	}
	if (note == DRUM_NOTE) {
		drum_hit();
	}					
}

*/

void note_on(int note, int velocity) {
	switch (note) {
		case TRIANGLE_NOTE: triangle_hit(); break;
		case SHAKER_NOTE:   shaker_hit();   break;
		case DRUM_NOTE:     drum_hit();     break;
	}					
}


void note_off(int note, int velocity) {
	// Nothing to do for percussion!
}


void test_servo() {
	triangle_servo.write(60);
	delay(500);
	triangle_servo.write(110);
	delay(500);
}


void self_test() {
	triangle_hit();
	shaker_hit();
	drum_hit();
}


void setup()
{	
	// Servo setup:
	triangle_servo.attach(SERVO_1_PIN);
	shaker_servo.attach(SERVO_2_PIN);
	drum_servo.attach(SERVO_3_PIN);
	
	// Set initial position to _MIN values
	triangle_servo.write(TRIANGLE_MIN);
	shaker_servo.write(SHAKER_MIN);
	drum_servo.write(DRUM_MIN);

	percussionBot.begin();
}


void loop()
{
	triangle_timer->Update();
	shaker_timer->Update();
	drum_timer->Update();
	
	percussionBot.process_MIDI();
}
