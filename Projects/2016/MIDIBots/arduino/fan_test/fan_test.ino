const int FAN_OUTPUT = 6;

void setup()
{
	pinMode(FAN_OUTPUT, OUTPUT);
	digitalWrite(FAN_OUTPUT, LOW);
}

void loop()
{
	analogWrite(FAN_OUTPUT, 64);
	delay(5000);
	analogWrite(FAN_OUTPUT, 0);
	delay(2500);
}
