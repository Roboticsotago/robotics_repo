// Testing multiple servo motors

#include <Servo.h> 
 
Servo servo1;
Servo servo2;

int pos = 0;    // variable to store the servo position 
const int MAX_POS = 100;
const int MIN_POS = 80;

const int SERVO_TIME = 50; // ms delay to allow motion to happen

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
 
void playbeat (Servo servo, int duration)
{
	servo.write(MAX_POS);
	delay(SERVO_TIME);
	servo.write(MIN_POS);
	delay(SERVO_TIME);
	delay(duration - 2 * SERVO_TIME);
}

void loop() {
	rhythm_4();
}

void rhythm_4()
{
	playbeat(servo1, 400);
	playbeat(servo1, 400);
	playbeat(servo1, 200);
	playbeat(servo1, 200);
	playbeat(servo2, 400);
	playbeat(servo1, 200);
	playbeat(servo2, 200);
	playbeat(servo2, 200);
	playbeat(servo1, 200);
}

void rhythm_3()
{
	playbeat(servo1, 200);
	playbeat(servo2, 100);
	playbeat(servo2, 100);
	playbeat(servo1, 100);
	playbeat(servo1, 100);
	playbeat(servo2, 200);
	playbeat(servo1, 200);
	playbeat(servo2, 200);
	playbeat(servo1, 200);
	playbeat(servo2, 100);
	playbeat(servo2, 100);
}

void rhythm_2()
{
	playbeat(servo1, 400);
	playbeat(servo2, 400);
	playbeat(servo1, 400);
	playbeat(servo2, 200);
	playbeat(servo1, 100);
	playbeat(servo2, 100);
}

void rhythm_1()
{
	playbeat(servo1, 200);
	playbeat(servo2, 200);

	playbeat(servo1, 200);
	playbeat(servo2, 200);

	playbeat(servo1, 200);
	playbeat(servo2, 200);

	playbeat(servo1, 100);
	playbeat(servo1, 100);
	playbeat(servo2, 100);
	playbeat(servo2, 100);
}
