// MIDI note tests for Mechatronics, CME 2016-03-21
// 1. Loop sanity check.  Watch for off-by-one errors!
// 2. Translate note frequency formula into C syntax and check (MIDI note 69 should give 440 Hz). 
// 3. Correct integer division in formula!

void setup() {
	delay(8000);  // safety delay for serial I/O
	Serial.begin(9600);
}

void loop() {
	for (int n = 0; n < 128; n++) {
		float f = 440 * pow(2, (n - 69) / 12.0);
		Serial.print(n);
		Serial.print(": ");
		Serial.println(f);
	}
	delay(5000);
}
