// Testing servo strumming ukulele

#include <Servo.h> 
 
Servo servo1;
Servo servo2;

int pos = 0;    // variable to store the servo position
const int CENTRE = 90;
const int RANGE = 46;
const int MAX_POS = CENTRE + RANGE/2;
const int MIN_POS = CENTRE - RANGE/2;

const int SERVO_TIME = 200; // ms delay to allow motion to happen

const int SERVO_1_PIN = 11;
const int SERVO_2_PIN = 10;

void setup() 
{ 
	pinMode(SERVO_1_PIN, OUTPUT);
	digitalWrite(SERVO_1_PIN, LOW);
	servo1.attach(SERVO_1_PIN);
	
	pinMode(SERVO_2_PIN, OUTPUT);
	digitalWrite(SERVO_2_PIN, LOW);
	servo2.attach(SERVO_2_PIN);	
} 
 
void strum()
{
	servo1.write(MAX_POS);
	delay(SERVO_TIME);
	servo1.write(MIN_POS);
	delay(SERVO_TIME);
}

void loop() {
	strum();
}
