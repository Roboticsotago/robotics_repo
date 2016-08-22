#include "Timer.h"
#include <Servo.h> 

Timer *phase_timer = new Timer(50); //starts a new timer

Servo servos [2];

int pos = 0;    // variable to store the servo position
const int CENTRE = 90;
const int RANGE = 46;
const int MAX_POS = CENTRE + RANGE/2;
const int MIN_POS = CENTRE - RANGE/2;

const int SERVO_TIME = 100; // ms delay to allow motion to happen
int phase = 0;


void setup() 
{ 
	servos[0].attach(9);
	servos[1].attach(10);
	
	phase_timer->setOnTimer(&hit); //sets new timer
	
	phase_timer->Start(); //starts new timer
} 

void hit(){
	phase = !phase;
	down_strum(servos[phase]);
	up_strum(servos[!phase]);
}	
void down_strum(Servo servo){
	for (int i = 0; i < 4; i++)
		servo.write(MAX_POS);
	
}


void up_strum(Servo servo){
	for (int i = 0; i < 4; i++)
		servo.write(MIN_POS);
	
}

void loop() {
	
	phase_timer->Update();
}
