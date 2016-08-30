// UkeBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

/*
TODO:
[ ] Calibrate slide positions to correspond to musical intervals
[ ] Add note-on messages for slide positioning
[ ] Add note-on messages for plucking indiviual strings (think of it as having 5 positions which can be reached: top, between first two strings, between middle strings, between last two strings, and bottom).  Could be tricky to get the timing right, as the time to pluck will depend on the current strumming position.  Perhaps just pick a single value that generally works OK.
[ ] Refactor to use arrays (2-dimensional?) for slide and strum (note, position pairs).
*/

// Define pin mappings for the MIDIBot shield:
#include <Servo.h> 
#include <MIDIBot.h>
MIDIBot UkeBot;

Servo strum;
Servo slide;

int pos = 0;    // variable to store the servo position

// Deprecated:
const int UP_NOTE = 50; // Middle C
const int DOWN_NOTE = 52; // D

// 1st attempt at strum positions, using midpoint between pairs of strings. Not reliable.
/*
const int
	STRUM_0_NOTE = 0,
	STRUM_1_NOTE = 1,
	STRUM_2_NOTE = 2,
	STRUM_3_NOTE = 3,
	STRUM_4_NOTE = 4;
*/
const int
	STRUM_0_NOTE = 0,
	STRUM_1_NOTE = 1,
	STRUM_2_NOTE = 2,
	STRUM_3_NOTE = 3,
	STRUM_4_NOTE = 4,
	STRUM_5_NOTE = 5,
	STRUM_6_NOTE = 6,
	STRUM_7_NOTE = 7,
	STRUM_8_NOTE = 8,
	STRUM_9_NOTE = 9;

const int
	SLIDE_0_NOTE = 60,
	SLIDE_1_NOTE = 61,
	SLIDE_2_NOTE = 62,
	SLIDE_3_NOTE = 63,
	SLIDE_4_NOTE = 64,
	SLIDE_5_NOTE = 65,
	SLIDE_6_NOTE = 66,
	SLIDE_7_NOTE = 67,
	SLIDE_8_NOTE = 68;
	
const int STRUM_MIN = 92;  //90 
const int STRUM_MAX = 97; //110

// Midpoint positions between strings. Doesn't result in reliable picking.
/*
const int
	STRUM_0 = 92,
	STRUM_1 = 96,
	STRUM_2 = 99,
	STRUM_3 = 102,
	STRUM_4 = 106;
*/

// Second attempt, using a pair of points either side of the midpoint for each string:
const int
	STRUM_0 = 92,
	STRUM_1 = 93,
	STRUM_2 = 95,
	STRUM_3 = 97,
	STRUM_4 = 98,
	STRUM_5 = 100,
	STRUM_6 = 101,
	STRUM_7 = 103,
	STRUM_8 = 105,
	STRUM_9 = 106;

const int
	SLIDE_0 = 29,
	SLIDE_1 = 37,
	SLIDE_2 = 45,
	SLIDE_3 = 52,
	SLIDE_4 = 59,
	SLIDE_5 = 65,
	SLIDE_6 = 71,
	SLIDE_7 = 77,
	SLIDE_8 = 83;
// For slide max/min, we probably want min to be at the nut end, and max at the soundhole end.
const int SLIDE_MIN = 29;
const int SLIDE_MAX = 83;
// SLIDE_HOME should no longer be needed, at least for anti-backlash, now that we have the new Lego-based slide linkage.  Probably sensible to zero it to SLIDE_MIN at startup.
//const int SLIDE_HOME = 50; // Somewhere far enough beyond MAX that it will avoid the backlash.  108
// TODO: constants (or an array?) of servo positions that correspond to musical notes.

// Keep track of last slide position set, in case we end up getting really fancy with the logic and trying to optimise the timings or something.
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
	// No longer needed with new Lego linkage
//	slide.write(SLIDE_HOME);
//	delay(SLIDE_DELAY);
	// Clip motion to valid range:
	if (pos < SLIDE_MIN) {pos = SLIDE_MIN;}
	if (pos > SLIDE_MAX) {pos = SLIDE_MAX;}
	slide.write(pos);
//	delay(SLIDE_DELAY);
}

void note_on(int note, int velocity) {
	switch (note) {
		case UP_NOTE:   up_strum();   break;
		case DOWN_NOTE: down_strum(); break;
		case STRUM_0_NOTE:	strum.write(STRUM_0); break;
		case STRUM_1_NOTE:	strum.write(STRUM_1); break;
		case STRUM_2_NOTE:	strum.write(STRUM_2); break;
		case STRUM_3_NOTE:	strum.write(STRUM_3); break;
		case STRUM_4_NOTE:	strum.write(STRUM_4); break;
		case STRUM_5_NOTE:	strum.write(STRUM_5); break;
		case STRUM_6_NOTE:	strum.write(STRUM_6); break;
		case STRUM_7_NOTE:	strum.write(STRUM_7); break;
		case STRUM_8_NOTE:	strum.write(STRUM_8); break;
		case STRUM_9_NOTE:	strum.write(STRUM_9); break;
		case SLIDE_0_NOTE:	move_slide(SLIDE_0); break;
		case SLIDE_1_NOTE:	move_slide(SLIDE_1); break;
		case SLIDE_2_NOTE:	move_slide(SLIDE_2); break;
		case SLIDE_3_NOTE:	move_slide(SLIDE_3); break;
		case SLIDE_4_NOTE:	move_slide(SLIDE_4); break;
		case SLIDE_5_NOTE:	move_slide(SLIDE_5); break;
		case SLIDE_6_NOTE:	move_slide(SLIDE_6); break;
		case SLIDE_7_NOTE:	move_slide(SLIDE_7); break;
		case SLIDE_8_NOTE:	move_slide(SLIDE_8); break;		
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
	slide.write(SLIDE_MIN);
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
