// UkeBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

/*
TODO:
[ ] Calibrate slide positions to correspond to musical intervals
[ ] Add note-on messages for slide positioning
[ ] Add note-on messages for plucking indiviual strings (think of it as having 5 positions which can be reached: top, between first two strings, between middle strings, between last two strings, and bottom).  Could be tricky to get the timing right, as the time to pluck will depend on the current strumming position.  Perhaps just pick a single value that generally works OK.
*/

// Define pin mappings for the MIDIBot shield:
#include <Servo.h> 
#include <MIDIBot.h>
MIDIBot UkeBot;

Servo strum;
Servo slide;

int pos = 0;    // variable to store the servo position

const int UP_NOTE = 60; // Middle C
const int DOWN_NOTE = 62; // D

const int STRUM_MIN = 90;
const int STRUM_MAX = 110;
// TODO: constants for each inter-string position
// ...

// For slide max/min, we probably want min to be at the nut end, and max at the soundhole end.
const int SLIDE_MIN = 60;	// 55 is too far - it can get stuck! 60
const int SLIDE_MAX = 88;	// 88
const int SLIDE_HOME = 108; // Somewhere far enough beyond MAX that it will avoid the backlash.  108
// TODO: constants (or an array?) of servo positions that correspond to musical notes.

int slide_position = SLIDE_MIN;

// Unused?
const int STRUM_DELAY = 100;

const int SLIDE_DELAY = 300;	// Worst-case slide motion time (HOME..MIN)


int i;
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

void test_slide() {
	slide.write(slide_position);
}

void move_slide(int pos) {
	// Move to home position first so it always approaches from the same direction (anti-backlash)
	slide.write(SLIDE_HOME);
	delay(SLIDE_DELAY);
	// Clip motion to valid range:
	if (pos < SLIDE_MIN) {pos = SLIDE_MIN;}
	if (pos > SLIDE_MAX) {pos = SLIDE_MAX;}
	slide.write(pos);
	delay(SLIDE_DELAY);
}

void note_on(int note, int velocity) {
	switch (note) {
		case UP_NOTE:   up_strum();   break;
		case DOWN_NOTE: down_strum(); break;
	}
}

void note_off(int note, int velocity) {}


void setup()
{
	UkeBot.begin();
	
	// Servo setup:
	strum.attach(SERVO_3_PIN);
	slide.attach(SERVO_2_PIN);
	
	// Set initial position
	strum.write(STRUM_MIN);
	slide.write(SLIDE_HOME);
}

void self_test_2()
{
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
}

void self_test() {
	digitalWrite(LED_PIN, HIGH);
	slide_position = SLIDE_MAX;
	for (i=0; i<=5; i++){
		move_slide(slide_position);
		up_strum();
		delay(500);
		down_strum();
		delay(500);
		slide_position = slide_position - 5;
	}
	digitalWrite(LED_PIN, LOW);
}

void loop()
{
	UkeBot.process_MIDI();
}
