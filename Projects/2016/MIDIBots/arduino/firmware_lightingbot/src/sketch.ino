// LightingBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago


// Define pin mappings for the MIDIBot shield:
const int
	MIDI_1x_PIN = 2,
	MIDI_2x_PIN = 3,
	MIDI_4x_PIN = 4,	/* USBDroid has pulldown resistor on D4! */
	MIDI_8x_PIN = 5,
	MOSFET_PWM_PIN = 6,
	SERVO_1_PIN = 9,	/* USBDroid has pulldown resistor on D9! */
	SERVO_2_PIN = 10,
	SERVO_3_PIN = 11,	/* */
	MOSFET_2_PIN = 12,
	MOSFET_3_PIN = A1,	/* not A3 as on the silkscreen! */
	MOSFET_4_PIN = A2,	/* not A4 as on the silkscreen! */
	LED_PIN = 13,		/* USBDroid has pulldown resistor on D13! */
	LATENCY_ADJUST_PIN = A0,
	SELF_TEST_PIN = A5
;

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


int pos = 0;    // variable to store the servo position


int MIDI_channel = 0;	// 1..16, 10=drums

// MIDI messages have up to 2 data bytes, which we store in an array:
int dataByte[2], statusByte = 0, i = 0;

void clearData(){
  dataByte[0] = 0;
  dataByte[1] = 0;
  i = 0;
}

// For monophonic instruments, it's useful to keep track of the note currently being played, so that any note-off messages for other notes can be ignored.
int current_note_number = 0;


void flash(int on_time, int off_time) {
	digitalWrite(LED_PIN, HIGH);
	delay(on_time);
	digitalWrite(LED_PIN, LOW);
	delay(off_time);	
}

// Routine for displaying a number by blinking the LED (since hardware serial will be taken already by the MIDI comms):
void flash_number(int n) {
	delay(500);
	// Hundreds:
	for (int i = 0; i < (n % 1000 / 100); i++) {
		flash(800, 800);
	}
	delay(500);
	// Tens:
	for (int i = 0; i < (n % 100 / 10); i++) {
		flash(400, 400);
	}
	delay(500);
	// Fastest flashing for the ones:
	for (int i = 0; i < (n % 10); i++) {
		flash(200, 200);
	}
	delay(1000);
}


// Read MIDI channel DIP switches and store the result:
// NOTE: Freetronics USBDroid has a pull-down resistor on pin D4, which makes that pin unusable! It acts as if D4 is always grounded, i.e. the switch is on.
// Multiply the 4x bit by 4 on a normal Arduino, or 0 to force it off.
void read_MIDI_channel() {
	MIDI_channel =
		!digitalRead(MIDI_8x_PIN) * 8 +
		!digitalRead(MIDI_4x_PIN) * 4 +
		!digitalRead(MIDI_2x_PIN) * 2 +
		!digitalRead(MIDI_1x_PIN);
}

void self_test() {
	// Robot-specific self-test routine goes here
//	read_MIDI_channel();
//	flash_number(MIDI_channel + 1);

	digitalWrite(BLUE_PIN, HIGH);
	digitalWrite(WHITE_PIN, LOW);
	delay(500);
	digitalWrite(BLUE_PIN, LOW);
	digitalWrite(WHITE_PIN, HIGH);
	delay(500);
	digitalWrite(WHITE_PIN, LOW);

	RGB_colour_test();
	
	RGB_fade_integer();
}

void setup()
{
	// Set up pin modes for the MIDIBot Shield (input, internal pull-up resistor enabled):
	pinMode(MIDI_1x_PIN, INPUT_PULLUP);
	pinMode(MIDI_2x_PIN, INPUT_PULLUP);
	pinMode(MIDI_4x_PIN, INPUT_PULLUP);
	pinMode(MIDI_8x_PIN, INPUT_PULLUP);
	
	// Ditto for self-test button:
	pinMode(SELF_TEST_PIN, INPUT_PULLUP);
	
	// TODO: output pins (servos, MOSFETs, LED)
	pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, LOW);
	pinMode(MOSFET_PWM_PIN, OUTPUT); analogWrite(MOSFET_PWM_PIN, 0);
	pinMode(MOSFET_2_PIN, OUTPUT); digitalWrite(MOSFET_2_PIN, LOW);
	pinMode(MOSFET_3_PIN, OUTPUT); digitalWrite(MOSFET_3_PIN, LOW);
	pinMode(MOSFET_4_PIN, OUTPUT); digitalWrite(MOSFET_4_PIN, LOW);	
	pinMode(SERVO_1_PIN, OUTPUT); digitalWrite(SERVO_1_PIN, LOW);
	pinMode(SERVO_2_PIN, OUTPUT); digitalWrite(SERVO_2_PIN, LOW);
	pinMode(SERVO_3_PIN, OUTPUT); digitalWrite(SERVO_3_PIN, LOW);
	
	// Initialise MIDI channel number according to DIP switch settings:
	read_MIDI_channel();
	flash_number(MIDI_channel);
	
	// Set up MIDI communication:
	Serial.begin(31250);
	clearData();
	
	// Attach interrupt for self-test button and function:
//	attachInterrupt(digitalPinToInterrupt(SELF_TEST_PIN), self_test, FALLING);
	// Only available on certain pins?  Just poll in main loop instead.
	
	// Flash to indicate startup/ready:
//	flash(50, 200); flash(50, 400); flash(50, 200); flash(400, 0);
	// Don't bother if flashing MIDI channel - it's confusing!
}

void loop()
{
	if (!digitalRead(SELF_TEST_PIN)) {
		self_test();
	}
	
	process_MIDI();

//	test_blink();
//	test_button();
//	test_flash_number();
//	test_MIDI_channel();
//	test_MOSFETs();
//	test_MOSFETs_cycle();
//	test_PWM();
//	test_servo();
//	RGB_blink();
//	RGB_fade_integer();
//	RGB_colour_test();
}


void process_MIDI() {
	if (Serial.available() > 0) {
		int data = Serial.read();
		if (data > 127) {
			// It's a status byte. Store it for future reference.
			statusByte = data;
			clearData();  
		} else {
			// It's a data byte.
			dataByte[i] = data;
			if (statusByte == (0x90 | MIDI_channel) && i == 1) {
				// Note-on message received
			//	flash(20,0); // Warning: blocks!
				if (dataByte[1] == 0) {
					// Stop note playing - nothing to do for percussion!
					if (dataByte[0] ==  BLUE_NOTE) {digitalWrite( BLUE_PIN, LOW);}
					if (dataByte[0] == WHITE_NOTE) {digitalWrite(WHITE_PIN, LOW);}
					if (dataByte[0] == R_NOTE) {analogWrite(R_PIN, 0);}
					if (dataByte[0] == G_NOTE) {analogWrite(G_PIN, 0);}
					if (dataByte[0] == B_NOTE) {analogWrite(B_PIN, 0);}
				} else {
					// Start note playing
					if (dataByte[0] ==  BLUE_NOTE) {digitalWrite( BLUE_PIN, HIGH);}
					if (dataByte[0] == WHITE_NOTE) {digitalWrite(WHITE_PIN, HIGH);}
					if (dataByte[0] == R_NOTE) {analogWrite(R_PIN, dataByte[1]*2);}
					if (dataByte[0] == G_NOTE) {analogWrite(G_PIN, dataByte[1]*2);}
					if (dataByte[0] == B_NOTE) {analogWrite(B_PIN, dataByte[1]*2);}
				}
			} else if (statusByte == (0x80 | MIDI_channel) && i == 1) {
				// Note-off message received
					if (dataByte[0] ==  BLUE_NOTE) {digitalWrite( BLUE_PIN, LOW);}
					if (dataByte[0] == WHITE_NOTE) {digitalWrite(WHITE_PIN, LOW);}
					if (dataByte[0] == R_NOTE) {analogWrite(R_PIN, 0);}
					if (dataByte[0] == G_NOTE) {analogWrite(G_PIN, 0);}
					if (dataByte[0] == B_NOTE) {analogWrite(B_PIN, 0);}
			}
			i++;
			// TODO: error detection if i goes beyond the array size.
		}
	}
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

void test_blink() {
	digitalWrite(LED_PIN, HIGH);
	delay(500);
	digitalWrite(LED_PIN, LOW);
	delay(500);
}

void test_button() {
	digitalWrite(LED_PIN, !digitalRead(SELF_TEST_PIN));
}

void test_MIDI_channel() {
	if (!digitalRead(SELF_TEST_PIN)) {
		read_MIDI_channel();
		Serial.print(MIDI_channel);
		Serial.print(" (");
		Serial.print(MIDI_channel + 1);
		Serial.println(")");
		flash_number(MIDI_channel + 1);
	}
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
