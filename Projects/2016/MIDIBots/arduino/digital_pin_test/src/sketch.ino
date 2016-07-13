// We've had so many issues with flaky digital pins that it's probably worth having a test program that toggles each one a few times for easy testing on the oscilloscope...


const int FIRST_PIN = 8;
const int LAST_PIN = 13;


void setup()
{
	for (int pin = FIRST_PIN; pin <= LAST_PIN; pin++) {
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}
}

void loop()
{
	for (int pin = FIRST_PIN; pin <= LAST_PIN; pin++) {
		multiflash(pin, 4);
	}
}

void multiflash(int pin, int times) {
	for (int i = 0; i < times; i++) {
		flash(pin);
	}
}

void flash(int pin) {
	digitalWrite(pin, HIGH);
	delay(100);
	digitalWrite(pin, LOW);
	delay(150);
}
