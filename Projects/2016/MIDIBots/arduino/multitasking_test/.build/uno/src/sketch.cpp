#include <Arduino.h>
#include "Timer.h"
#include <Servo.h> 
void setup();
void down_strum();
void up_strum();
void loop();
#line 1 "src/sketch.ino"
//#include "Timer.h"
//#include <Servo.h> 

Timer *servo_0_timer = new Timer(300); //starts a new timer
Timer *servo_1_timer = new Timer(250); //starts a new timer

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

	servo_0_timer->setOnTimer(&up_strum); //sets new timer
	servo_1_timer->setOnTimer(&down_strum); //sets new timer
	servo_0_timer->Start(); //starts new timer
	servo_1_timer->Start(); //starts new timer
} 

void down_strum(){
	for (int i = 0; i < 6; i++)
		servos[i].write(MAX_POS);
	
}


void up_strum(){
	for (int i = 0; i < 6; i++)
		servos[i].write(MIN_POS);
	
}

void loop() {
	
	servo_1_timer->Update();
	servo_0_timer->Update();
}
