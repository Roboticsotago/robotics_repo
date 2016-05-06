// MIDI note tests for Mechatronics, CME 2016-03-21
// 1. Loop sanity check.  Watch for off-by-one errors!
// 2. Translate note frequency formula into C syntax and check (MIDI note 69 should give 440 Hz). 
// 3. Correct integer division in formula!
// 4. Derive OCR "top" value from frequency, and print for checking.
// 5. Correct type of "top" to get the full available range.

void setup() {
	delay(8000);  // safety delay for serial I/O
	Serial.begin(9600);
}

void loop() {
	int prescale = 8;
	for (int n = 0; n < 128; n++) {
		float f = 440 * pow(2, (n - 69) / 12.0);
		unsigned int top = round(F_CPU / (prescale * f) - 1);
		Serial.print(n);
		Serial.print(": f=");
		Serial.print(f);
		Serial.print(", top=");
		Serial.println(top);
	}
	while (1) {}
}
