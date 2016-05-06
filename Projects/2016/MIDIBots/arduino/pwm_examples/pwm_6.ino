// MIDI note tests for Mechatronics, CME 2016-03-21
// 1. Loop sanity check.  Watch for off-by-one errors!
// 2. Translate note frequency formula into C syntax and check (MIDI note 69 should give 440 Hz). 
// 3. Correct integer division in formula!
// 4. Derive OCR "top" value from frequency, and print for checking.
// 5. Correct type of "top" to get the full available range.
// 6. Move the formulae into separate functions, and check that it still works!

int prescale = 8;

float note_to_frequency(int n) {
	return 440 * pow(2, (n - 69) / 12.0);
}

unsigned int frequency_to_counter_limit(float f) {
	return round(F_CPU / (prescale * f) - 1);
}
	
void setup() {
	delay(8000);  // safety delay for serial I/O
	Serial.begin(9600);
}

void loop() {
	int prescale = 8;
	for (int n = 0; n < 128; n++) {
		float f = note_to_frequency(n);
		unsigned int top = frequency_to_counter_limit(f);
		Serial.print(n);
		Serial.print(": f=");
		Serial.print(f);
		Serial.print(", top=");
		Serial.println(top);
	}
	while (1) {}
}


