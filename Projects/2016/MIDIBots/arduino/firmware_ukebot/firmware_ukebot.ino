// UkeBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

/*
TODO:
[ ] Consider refactoring to use Servo.writeMicroseconds() for more accurate control over positioning. Maybe add a wrapper function that accepts target angle as a float.
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

// Servo angles for strumming...

const int STRUM_MIN = 92; 
const int STRUM_MAX = 105;

const int
	STRUM_0 = 92,	// OK
	STRUM_1 = 93,	//
	STRUM_2 = 95,	// 
	STRUM_3 = 97,	// 
	STRUM_4 = 98,	//
	STRUM_5 = 100,	// 
	STRUM_6 = 101,	// OK
	STRUM_7 = 103,	// Maybe 102 - sometimes hits the next string
	STRUM_8 = 104,	// 
	STRUM_9 = 105;	// OK

// ..and the slide.
// For slide max/min, we probably want min to be at the nut end, and max at the soundhole end.
const int SLIDE_MIN = 29;
const int SLIDE_MAX = 83;

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

// MIDI note numbers for strumming...
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

// ...and picking:
const int
	SLIDE_0_NOTE = 12,
	SLIDE_1_NOTE = 13,
	SLIDE_2_NOTE = 14,
	SLIDE_3_NOTE = 15,
	SLIDE_4_NOTE = 16,
	SLIDE_5_NOTE = 17,
	SLIDE_6_NOTE = 18,
	SLIDE_7_NOTE = 19,
	SLIDE_8_NOTE = 20;

// TODO: constants (or an array?) of servo positions that correspond to musical notes.

// Keep track of last slide position set, in case we end up getting really fancy with the logic and trying to optimise the timings or something.
int slide_position = SLIDE_MIN;

// Unused?
const int STRUM_DELAY = 100;

const int SLIDE_DELAY = 300;	// Worst-case slide motion time (HOME..MIN). Unconfirmed!


int i;

void move_slide(int pos) {
	// Clip motion to valid range:
	if (pos < SLIDE_MIN) {pos = SLIDE_MIN;}
	if (pos > SLIDE_MAX) {pos = SLIDE_MAX;}
	// Move it:
	slide.write(pos);
}

// TODO: similar to move_slide() but for picking, maybe?

void note_on(int note, int velocity) {
	switch (note) {
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

void self_test_picking() {
	strum.write(STRUM_1); delay(500);
	strum.write(STRUM_0); delay(500);
	strum.write(STRUM_3); delay(500);
	strum.write(STRUM_2); delay(500);
	strum.write(STRUM_5); delay(500);
	strum.write(STRUM_4); delay(500);
	strum.write(STRUM_7); delay(500);
	strum.write(STRUM_6); delay(500);
	strum.write(STRUM_9); delay(500);
	strum.write(STRUM_8); delay(500);

	strum.write(STRUM_8); delay(500);
	strum.write(STRUM_9); delay(500);
	strum.write(STRUM_6); delay(500);
	strum.write(STRUM_7); delay(500);
	strum.write(STRUM_4); delay(500);
	strum.write(STRUM_5); delay(500);
	strum.write(STRUM_2); delay(500);
	strum.write(STRUM_3); delay(500);
	strum.write(STRUM_0); delay(500);
	strum.write(STRUM_1); delay(500);
	
	strum.write(STRUM_0); delay(500);	// Return home
}

void self_test() {
	// TODO: test each semitone, not just min/max slide position. This would be easier if we used an array for the slide positions.
	digitalWrite(LED_PIN, HIGH);
	move_slide(SLIDE_MIN); delay(SLIDE_DELAY);
	self_test_picking();
	move_slide(SLIDE_MAX); delay(SLIDE_DELAY);
	self_test_picking();
	move_slide(SLIDE_MIN); delay(SLIDE_DELAY);
	digitalWrite(LED_PIN, LOW);
}

void loop()
{
	UkeBot.process_MIDI();
}
