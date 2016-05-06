// MIDI note tests for Mechatronics, CME 2016-03-21
// 1. Loop sanity check.  Watch for off-by-one errors!

void setup() {
	delay(8000);  // safety delay for serial I/O
	Serial.begin(9600);
}

void loop() {
	for (int i = 0; i < 128; i++) {
		Serial.println(i);
	}
	while (1) {}
}
