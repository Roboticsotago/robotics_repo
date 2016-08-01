// ____Bot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

#include <MIDIBot.h>
MIDIBot thisMIDIBot;

// Additional constants for the lighting setup:
const int BLUE_PIN = MOSFET_4_PIN;
const int WHITE_PIN = MOSFET_3_PIN;

const int BLUE_NOTE = 60; // MIDI middle C
const int WHITE_NOTE = 62; // MIDI D above middle C

// RGB LED strip setup:
const int R_PIN = SERVO_1_PIN; // red
const int G_PIN = SERVO_2_PIN; // green
const int B_PIN = SERVO_3_PIN; // blue

// RGB LED strip control notes:
const int R_NOTE = 67;	// G above middle C
const int G_NOTE = 69;	// A above middle C
const int B_NOTE = 71;  // B above middle C


void note_on(int note, int velocity) {
	if (note ==  BLUE_NOTE) {digitalWrite( BLUE_PIN, HIGH);}
	if (note == WHITE_NOTE) {digitalWrite(WHITE_PIN, HIGH);}
	if (note == R_NOTE) {analogWrite(R_PIN, velocity*2);}
	if (note == G_NOTE) {analogWrite(G_PIN, velocity*2);}
	if (note == B_NOTE) {analogWrite(B_PIN, velocity*2);}
}

void note_off(int note, int velocity) {
	if (note ==  BLUE_NOTE) {digitalWrite( BLUE_PIN, LOW);}
	if (note == WHITE_NOTE) {digitalWrite(WHITE_PIN, LOW);}
	if (note == R_NOTE) {analogWrite(R_PIN, 0);}
	if (note == G_NOTE) {analogWrite(G_PIN, 0);}
	if (note == B_NOTE) {analogWrite(B_PIN, 0);}
}

void self_test() {

	digitalWrite(BLUE_PIN, HIGH);
	digitalWrite(WHITE_PIN, LOW);
	delay(500);
	digitalWrite(BLUE_PIN, LOW);
	digitalWrite(WHITE_PIN, HIGH);
	delay(500);
	digitalWrite(WHITE_PIN, LOW);
        Serial.print("hello");
	RGB_colour_test();
	
	RGB_fade_integer();
	
}

void setup() {
        delay(5000);
        Serial.begin(9600);
	thisMIDIBot.test_MIDI_channel();	// Indicate MIDI channel at startup
}

void loop() {
	if (!digitalRead(SELF_TEST_PIN)) {
		self_test();
	}
	thisMIDIBot.process_MIDI();
}


const float EXP = 2.0; // Stevens power-law brightness exponent

// Human perception of brightness is non-linear. This function attempts to compensate and provide linear control over perceived brightness.  Input and output are (assumed to be) normalised to the range 0..1.
float brightness(float level) {
	// Exponential (to compensate for logarithmic response):
	// TODO: ...
	// Stevens (power-law) model:
	return pow(level, EXP);	// Min. step size = (1/255)^(1/exponent)
}

// Compute minimum step size to achive full output resolution with 0..255 PWM (STEP computed WRT 0..1 range):
//const float STEP = pow(1.0 / 255.0, 1.0 / EXP); // Stevens
const float STEP = 0.0001;

// Plus another function to quantise to the 0..255 levels provided by the PWM output:
int quantise(float value) {
	return (int) round(value * 255.0);
}

void RGB_blink() {
	digitalWrite(R_PIN, HIGH);
	digitalWrite(G_PIN, LOW);
	digitalWrite(B_PIN, LOW);
	delay(150);

	digitalWrite(R_PIN, LOW);
	digitalWrite(G_PIN, HIGH);
	digitalWrite(B_PIN, LOW);
	delay(150);

	digitalWrite(R_PIN, LOW);
	digitalWrite(G_PIN, LOW);
	digitalWrite(B_PIN, HIGH);
	delay(150);

	digitalWrite(R_PIN, LOW);
	digitalWrite(G_PIN, LOW);
	digitalWrite(B_PIN, LOW);
//	delay(1500);
}

long r, g, b;
const int FADE_DELAY = 1;

void RGB_fade_integer() {
	digitalWrite(R_PIN, LOW);
	digitalWrite(G_PIN, LOW);
	digitalWrite(B_PIN, LOW);
	
	// Fade in:
	for (r = 0; r < 256; r++) {
		analogWrite(R_PIN, r);
		delay(FADE_DELAY);
	}
	for (g = 0; g < 256; g++) {
		analogWrite(G_PIN, g);
		delay(FADE_DELAY);
	}
	for (b = 0; b < 256; b++) {
		analogWrite(B_PIN, b);
		delay(FADE_DELAY);
	}

	// Fade out:
	for (r = 255; r >= 0; r--) {
		analogWrite(R_PIN, r);
		delay(FADE_DELAY);
	}
	for (g = 255; g >= 0; g--) {
		analogWrite(G_PIN, g);
		delay(FADE_DELAY);
	}
	for (b = 255; b >= 0; b--) {
		analogWrite(B_PIN, b);
		delay(FADE_DELAY);
	}
}

// TODO: finish testing this and complete it:
void RGB_fade() {
	digitalWrite(R_PIN, LOW);
	digitalWrite(G_PIN, LOW);
	digitalWrite(B_PIN, LOW);
	
	// Fade in:
//	for (r = 0; r < 256; r++) {
//		analogWrite(R_PIN, quantise(brightness(r));
//		delay(FADE_DELAY);
//	}
	for (g = 0; g < (long) round(1.0/STEP); g++) {
		analogWrite(G_PIN, quantise(brightness(g * STEP)));
	//	delay(FADE_DELAY);
	}
//	for (b = 0; b < 256; b++) {
//		analogWrite(B_PIN, b);
//		delay(FADE_DELAY);
//	}

	// Fade out:
//	for (r = 255; r >= 0; r--) {
//		analogWrite(R_PIN, r);
//		delay(FADE_DELAY);
//	}
	for (g = (long) round(1.0/STEP); g >= 0; g--) {
		analogWrite(G_PIN, quantise(brightness(g * STEP)));
	//	delay(FADE_DELAY);
	}
//	for (b = 255; b >= 0; b--) {
//		analogWrite(B_PIN, b);
//		delay(FADE_DELAY);
//	}
}

void RGB_colour_test() {
	const int DELAY = 500;
	RGB_write(255,   0,   0); delay(DELAY);	// red
	RGB_write(  0, 255,   0); delay(DELAY);	// green
	RGB_write(  0,   0, 255); delay(DELAY);	// blue
	RGB_write(  0, 255, 255); delay(DELAY);	// cyan
	RGB_write(255,   0, 255); delay(DELAY);	// magenta
	RGB_write(255, 255,   0); delay(DELAY);	// yellow
	RGB_write(255, 255, 255); delay(DELAY);	// white
	delay(DELAY);
}

void RGB_write(int R, int G, int B) {
	analogWrite(R_PIN, R);
	analogWrite(G_PIN, G);
	analogWrite(B_PIN, B);
}

