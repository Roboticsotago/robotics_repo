// Template/skeleton code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago


// Define pin mappings for the MIDIBot shield:
const int
	MIDI_1x_PIN = 2,
	MIDI_2x_PIN = 3,
	MIDI_4x_PIN = 4,
	MIDI_8x_PIN = 5,
	MOSFET_PWM_PIN = 6,
	SERVO_1_PIN = 9,
	SERVO_2_PIN = 10,
	SERVO_3_PIN = 11,
	MOSFET_2_PIN = 12,
	MOSFET_3_PIN = A1, /* not A3 as on the silkscreen! */
	MOSFET_4_PIN = A2, /* not A4 as on the silkscreen! */
	LED_PIN = 13,
	LATENCY_ADJUST_PIN = A0,
	SELF_TEST_PIN = A5
;

int
	MIDI_channel = 0;	// 1..16, 10=drums

// MIDI messages have up to 2 data bytes, which we store in an array:
int dataByte[2], statusByte = 0, i = 0;

void clearData(){
  dataByte[0] = 0;
  dataByte[1] = 0;
  i = 0;
}

// Even though this is only monophonic, it's still useful to keep track of the note currently being played, so that any note-off messages for other notes can be ignored.
int current_note_number = 0;


void flash(int on_time, int off_time) {
	digitalWrite(LED_PIN, HIGH);
	delay(on_time);
	digitalWrite(LED_PIN, LOW);
	delay(off_time);	
}

// Routine for displaying a number by blinking the LED (since hardware serial will be taken already by the MIDI comms):
void flash_number(int n) {
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
}


// Read MIDI channel DIP switches and store the result:
// NOTE: Freetronics USBDroid has a pull-down resistor on pin D4, which makes that pin unusable! It acts as if D4 is always grounded, i.e. the switch is on.
void read_MIDI_channel() {
	MIDI_channel =
		!digitalRead(MIDI_8x_PIN) * 8 +
		!digitalRead(MIDI_4x_PIN) * 4 +
		!digitalRead(MIDI_2x_PIN) * 2 +
		!digitalRead(MIDI_1x_PIN);
}

void self_test() {
	// Robot-specific self-test routine goes here
}

// TODO: interrupt handling for pushing the self-test button

void setup()
{
	// Set up pin modes for the MIDIBot Shield (input, internal pull-up resistor enabled):
	pinMode(MIDI_1x_PIN, INPUT); digitalWrite(MIDI_1x_PIN, HIGH);
	pinMode(MIDI_2x_PIN, INPUT); digitalWrite(MIDI_2x_PIN, HIGH);
	pinMode(MIDI_4x_PIN, INPUT); digitalWrite(MIDI_4x_PIN, HIGH);
	pinMode(MIDI_8x_PIN, INPUT); digitalWrite(MIDI_8x_PIN, HIGH);
	
	// Ditto for self-test button:
	pinMode(SELF_TEST_PIN, INPUT); digitalWrite(SELF_TEST_PIN, HIGH);
	
	// TODO: output pins (servos, MOSFETs, LED)
	pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN, LOW);
	pinMode(MOSFET_2_PIN, OUTPUT); digitalWrite(MOSFET_2_PIN, LOW);
	pinMode(MOSFET_3_PIN, OUTPUT); digitalWrite(MOSFET_3_PIN, LOW);
	pinMode(MOSFET_4_PIN, OUTPUT); digitalWrite(MOSFET_4_PIN, LOW);	
	
	// Initialise MIDI channel number according to DIP switch settings:
	read_MIDI_channel();
	
	// Set up MIDI communication:
	Serial.begin(31250);
	clearData();

	// Flash to indicate startup/ready:
	flash(50, 200); flash(50, 400); flash(50, 200); flash(400, 0);
}

void loop()
{
	process_MIDI();
//	test_blink();
//	test_button();
//	test_flash_number();
//	test_MIDI_channel();
//	test_MOSFETs();
//	test_MOSFETs_cycle();
//	test_PWM();
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
				if (dataByte[1] == 0 && dataByte[0] == current_note_number) {
					// Stop note playing
				//	pwm_off();
					digitalWrite(LED_PIN, LOW);
					analogWrite(MOSFET_PWM_PIN, 0);
				} else {
					// Start note playing
					current_note_number = dataByte[0];
//					pwm(frequency(current_note_number), dataByte[1] / 127.0 / 2); // TODO: map velocity to PWM duty
					digitalWrite(LED_PIN, HIGH);
					analogWrite(MOSFET_PWM_PIN, 64);
				//	drum();
				}
			} else if (statusByte == (0x80 | MIDI_channel) && i == 1 && dataByte[0] == current_note_number) {
				// Note-off message received
				// TODO: also respond to note-on with vel=0 as note-off
				// Stop note playing
				digitalWrite(LED_PIN, LOW);
				analogWrite(MOSFET_PWM_PIN, 0);
			//	pwm_off();
			}
			i++;
			// TODO: error detection if i goes beyond the array size.
		}
	}
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

void test_flash_number() {
	if (!digitalRead(SELF_TEST_PIN)) {
		flash_number(3);
		delay(2000);
		flash_number(16);
		delay(2000);
		flash_number(235);
		delay(2000);
		flash_number(127);
		delay(2000);
	}
}

void test_MIDI_channel() {
	if (!digitalRead(SELF_TEST_PIN)) {
		read_MIDI_channel();
		Serial.println(MIDI_channel);
		flash_number(MIDI_channel);
	}
}

void test_MOSFETs() {
	if (!digitalRead(SELF_TEST_PIN)) {
		analogWrite(MOSFET_PWM_PIN, 64);
		digitalWrite(MOSFET_2_PIN, HIGH);
		digitalWrite(MOSFET_3_PIN, HIGH);
		digitalWrite(MOSFET_4_PIN, HIGH);
	} else {
		analogWrite(MOSFET_PWM_PIN, 0);
		digitalWrite(MOSFET_2_PIN, LOW);
		digitalWrite(MOSFET_3_PIN, LOW);
		digitalWrite(MOSFET_4_PIN, LOW);
	}
}

void test_MOSFETs_cycle() {
	digitalWrite(MOSFET_PWM_PIN, HIGH);
	delay(250);
	digitalWrite(MOSFET_PWM_PIN, LOW);
	
	digitalWrite(MOSFET_2_PIN, HIGH);
	delay(250);
	digitalWrite(MOSFET_2_PIN, LOW);
	
	digitalWrite(MOSFET_3_PIN, HIGH);
	delay(250);
	digitalWrite(MOSFET_3_PIN, LOW);

	digitalWrite(MOSFET_4_PIN, HIGH);
	delay(250);
	digitalWrite(MOSFET_4_PIN, LOW);
}

void test_PWM() {
	analogWrite(MOSFET_PWM_PIN, 64);
	delay(250);
	analogWrite(MOSFET_PWM_PIN, 0);
	delay(500);
}
