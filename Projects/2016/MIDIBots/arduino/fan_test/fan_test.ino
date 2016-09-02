// FanBot speed test code
// Control fan speed using serial data
// "+" to increase speed
// "-" to decrease speed


const int FAN_OUTPUT = 6,
	MAX_SPEED = 255,
	MIN_SPEED = 0;

int fan_speed = 0;
int control_byte = 0;

void setup()
{
	pinMode(FAN_OUTPUT, OUTPUT);
	analogWrite(FAN_OUTPUT, LOW);
	Serial.begin(9600);
}

void speed_increase() {
	fan_speed++;
	if (fan_speed > MAX_SPEED) {fan_speed = MAX_SPEED;}
	analogWrite(FAN_OUTPUT, fan_speed);
}

void speed_decrease() {
	fan_speed--;
	if (fan_speed < MIN_SPEED) {fan_speed = MIN_SPEED;}
	analogWrite(FAN_OUTPUT, fan_speed);
}

void loop()
{
	if (Serial.available()) {
		control_byte = Serial.read();
		// TODO: error-checking?
		switch (control_byte) {
			case '+': speed_increase(); break;
			case '-': speed_decrease(); break;
		}
		Serial.println(fan_speed);
	}
}
