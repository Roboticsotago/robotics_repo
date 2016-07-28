// Testing servo strumming ukulele

#include <Servo.h> 

Servo servos [1];
int pos = 0;    // variable to store the servo position
const int CENTRE = 90;
const int RANGE = 20;
const int MAX_POS = CENTRE + RANGE/2;
const int MIN_POS = CENTRE - RANGE/2;

const int SERVO_TIME = 500; // ms delay to allow motion to happen


void setup() 
{
	servos[0].attach(11);
} 
 
void strum(Servo servo)
{
	servo.write(MAX_POS);
	delay(SERVO_TIME);
	servo.write(MIN_POS);
	delay(SERVO_TIME);
}

void loop() {
	strum(servos[0]);
}
