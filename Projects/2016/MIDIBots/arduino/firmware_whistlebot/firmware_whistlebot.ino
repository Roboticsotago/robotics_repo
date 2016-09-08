// WhistleBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

// Define pin mappings for the MIDIBot shield:
#include <Servo.h> 
#include <MIDIBot.h>
MIDIBot whistleBot;

Servo valve;
Servo slide;

int pos = 0;    // variable to store the servo position

const int VALVE_MIN = 92; 
const int VALVE_MAX = 105;

const int
	VALVE_0 = 92,	// OK
	VALVE_1 = 93,	//
	VALVE_2 = 95,	// 
	VALVE_3 = 97,	// 
	VALVE_4 = 98,	//
	VALVE_5 = 100,	// 
	VALVE_6 = 101,	// OK
	VALVE_7 = 103,	// Maybe 102 - sometimes hits the next string
	VALVE_8 = 104,	// 
	VALVE_9 = 105;	// OK

// ..and the slide.
// For slide max/min, we probably want min to be at the nut end, and max at the soundhole end.
const int SLIDE_MIN = 29; //29
const int SLIDE_MAX = 83; //83

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

// MIDI note numbers for valveming...
const int
	VALVE_0_NOTE = 0,
	VALVE_1_NOTE = 1,
	VALVE_2_NOTE = 2,
	VALVE_3_NOTE = 3,
	VALVE_4_NOTE = 4,
	VALVE_5_NOTE = 5,
	VALVE_6_NOTE = 6,
	VALVE_7_NOTE = 7,
	VALVE_8_NOTE = 8,
	VALVE_9_NOTE = 9;

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
const int VALVE_DELAY = 100;

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
		case VALVE_0_NOTE:	valve.write(VALVE_0); break;
		case VALVE_1_NOTE:	valve.write(VALVE_1); break;
		case VALVE_2_NOTE:	valve.write(VALVE_2); break;
		case VALVE_3_NOTE:	valve.write(VALVE_3); break;
		case VALVE_4_NOTE:	valve.write(VALVE_4); break;
		case VALVE_5_NOTE:	valve.write(VALVE_5); break;
		case VALVE_6_NOTE:	valve.write(VALVE_6); break;
		case VALVE_7_NOTE:	valve.write(VALVE_7); break;
		case VALVE_8_NOTE:	valve.write(VALVE_8); break;
		case VALVE_9_NOTE:	valve.write(VALVE_9); break;

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
//	UkeBot.begin();
	Serial.begin(9600);
	// Servo setup:
	valve.attach(SERVO_3_PIN);
	slide.attach(SERVO_2_PIN);
	
	// Set initial position
	valve.write(VALVE_MIN);
	slide.write(126);
}

void self_test_picking() {
	valve.write(VALVE_1); delay(500);
	valve.write(VALVE_0); delay(500);
	valve.write(VALVE_3); delay(500);
	valve.write(VALVE_2); delay(500);
	valve.write(VALVE_5); delay(500);
	valve.write(VALVE_4); delay(500);
	valve.write(VALVE_7); delay(500);
	valve.write(VALVE_6); delay(500);
	valve.write(VALVE_9); delay(500);
	valve.write(VALVE_8); delay(500);

	valve.write(VALVE_8); delay(500);
	valve.write(VALVE_9); delay(500);
	valve.write(VALVE_6); delay(500);
	valve.write(VALVE_7); delay(500);
	valve.write(VALVE_4); delay(500);
	valve.write(VALVE_5); delay(500);
	valve.write(VALVE_2); delay(500);
	valve.write(VALVE_3); delay(500);
	valve.write(VALVE_0); delay(500);
	valve.write(VALVE_1); delay(500);
	
	valve.write(VALVE_0); delay(500);	// Return home
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

int angle = 126;

void angle_increase() {
	angle++;
	if (angle > 180) {angle = 180;}
	slide.write(angle);
}

void angle_decrease() {
	angle--;
	if (angle < 0) {angle = 0;}
	slide.write(angle);
	
}

int control_byte;

void loop()
{
	if (Serial.available()) {
		control_byte = Serial.read();
		// TODO: error-checking?
		switch (control_byte) {
			case '+': angle_increase(); break;
			case '-': angle_decrease(); break;
			case ' ': slide.write(180); break;
		}
		Serial.println(angle);
	}
}

/*
void loop()
{
	UkeBot.process_MIDI();
}
*/
