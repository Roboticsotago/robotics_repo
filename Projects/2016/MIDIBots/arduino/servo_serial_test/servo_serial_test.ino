#include <Servo.h> 

Servo servo;

int angle = 90;

void setup()
{
	Serial.begin(9600);
	// Servo setup:
	servo.attach(10);
	
	// Set initial position
	servo.write(angle);
}

void angle_increase() {
	angle++;
	if (angle > 180) {angle = 180;}
	servo.write(angle);
}

void angle_decrease() {
	angle--;
	if (angle < 0) {angle = 0;}
	servo.write(angle);
	
}

int control_byte;

void loop()
{
	if (Serial.available()) {
		control_byte = Serial.read();
		// TODO: error-checking?
		switch (control_byte) {
			case '+': angle_increase(); break;
			case '=': angle_increase(); break;
			case '-': angle_decrease(); break;
			case '_': angle_decrease(); break;
		}
		Serial.println(angle);
	}
}
