#include <Arduino.h>
#include <Servo.h> 
void setup();
void strum(Servo servo);
void loop();
#line 1 "src/sketch.ino"
// Testing servo strumming ukulele

//#include <Servo.h> 

Servo servos [6];
int pos = 0;    // variable to store the servo position
const int CENTRE = 90;
const int RANGE = 46;
const int MAX_POS = CENTRE + RANGE/2;
const int MIN_POS = CENTRE - RANGE/2;

const int SERVO_TIME = 100; // ms delay to allow motion to happen


void setup() 
{ 
	servos[0].attach(3);
	servos[1].attach(5);
	servos[2].attach(6);
	servos[3].attach(9);
	servos[4].attach(10);
	servos[5].attach(11);

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
	strum(servos[1]);
	strum(servos[2]);
	strum(servos[3]);
	strum(servos[4]);
	strum(servos[5]);

}
