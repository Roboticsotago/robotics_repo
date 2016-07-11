const int TEST_PIN = 10;


void setup()
{
	pinMode(TEST_PIN, OUTPUT);
	digitalWrite(TEST_PIN, LOW);
}

void loop()
{
	digitalWrite(TEST_PIN, HIGH);
	delay(500);
	digitalWrite(TEST_PIN, LOW);
	delay(500);
}
