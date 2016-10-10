//#include <line_follower.h>
//#include <robot.h>
//#include <seeed.h>

// Simple demonstration LED blinker for the DSpace Robot board
// The onboard LEDs are on digital pins 6-10.
//  D6: LED1, red
//  D7: LED2, orange
//  D8: LED3, yellow
//  D9: LED4, green
// D10: LED5, blue

// Note that we have generally modified our boards to allow PWM control of both H-bridge channels.  On the stock boards, pins 4 and 5 are enable, but pin 4 has no PWM support.  Also 6,7 motor A, 9,10 motor B.

// Left motor is on pin 0, right motor is on pin 1.
// RobotMotor class provides Init(), Stop(), Brake(), Forwards(), Backwards().
// RobotDrive is for higher-level control.  Doesn't have a turn-on-the spot behaviour.  We probably want PWM control over speed as well, so we might have to devise our own library for this.
// Might not even have to be hardcoded for the DSpace Robot board - perhaps any dual H-bridge configuration.

// Change pin 4 to 11 for our modified DSpace boards (PWM on fwd)

#define MOTOR_R_ENABLE 11
#define MOTOR_L_ENABLE 5

#define MOTOR_R_1_PIN 6
#define MOTOR_R_2_PIN 7

#define MOTOR_L_1_PIN 9
#define MOTOR_L_2_PIN 10

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

const int MOTOR_L_DUTY=180;
const int MOTOR_R_DUTY=150;
//#include <line_follower.h>
//#include <robot.h>
//#include <seeed.h>

//static const int LED_PIN = 10;

void setup() {
	pinMode(MOTOR_L_ENABLE, OUTPUT); digitalWrite(MOTOR_L_ENABLE, LOW);
	pinMode(MOTOR_R_ENABLE, OUTPUT); digitalWrite(MOTOR_R_ENABLE, LOW);

	pinMode(MOTOR_L_1_PIN, OUTPUT); digitalWrite(MOTOR_L_1_PIN, LOW);
	pinMode(MOTOR_L_2_PIN, OUTPUT); digitalWrite(MOTOR_L_2_PIN, HIGH);

	pinMode(MOTOR_R_1_PIN, OUTPUT); digitalWrite(MOTOR_R_1_PIN, LOW);
	pinMode(MOTOR_R_2_PIN, OUTPUT); digitalWrite(MOTOR_R_2_PIN, HIGH);
}

// Low-level functions for driving the L and R motors independently...


void L_Fwd() {
	digitalWrite(MOTOR_L_1_PIN, LOW);
	digitalWrite(MOTOR_L_2_PIN, HIGH);
	analogWrite(MOTOR_L_ENABLE, 120);
//	digitalWrite(MOTOR_L_ENABLE, HIGH);
}

void L_Rev() {
	digitalWrite(MOTOR_L_1_PIN, HIGH);
	digitalWrite(MOTOR_L_2_PIN, LOW);
	analogWrite(MOTOR_L_ENABLE, MOTOR_L_DUTY);
//	digitalWrite(MOTOR_L_ENABLE, HIGH);
}

void L_Stop() {
	digitalWrite(MOTOR_L_1_PIN, LOW);
	digitalWrite(MOTOR_L_2_PIN, HIGH);
	analogWrite(MOTOR_L_ENABLE, 0);
//	digitalWrite(MOTOR_L_ENABLE, LOW);
}

void L_Brake() {
	digitalWrite(MOTOR_L_1_PIN, HIGH);
	digitalWrite(MOTOR_L_2_PIN, HIGH);
	analogWrite(MOTOR_L_ENABLE, MOTOR_L_DUTY);
//	digitalWrite(MOTOR_L_ENABLE, HIGH);
}


void R_Fwd() {
	digitalWrite(MOTOR_R_1_PIN, LOW);
	digitalWrite(MOTOR_R_2_PIN, HIGH);
	analogWrite(MOTOR_R_ENABLE, MOTOR_L_DUTY);
//	digitalWrite(MOTOR_R_ENABLE, HIGH);
}

void R_Rev() {
	digitalWrite(MOTOR_R_1_PIN, HIGH);
	digitalWrite(MOTOR_R_2_PIN, LOW);
	analogWrite(MOTOR_R_ENABLE, MOTOR_R_DUTY);
//	digitalWrite(MOTOR_R_ENABLE, HIGH);
}

void R_Stop() {
	digitalWrite(MOTOR_R_1_PIN, LOW);
	digitalWrite(MOTOR_R_2_PIN, HIGH);
	analogWrite(MOTOR_R_ENABLE, 0);
//	digitalWrite(MOTOR_R_ENABLE, LOW);
}

void R_Brake() {
	digitalWrite(MOTOR_R_1_PIN, HIGH);
	digitalWrite(MOTOR_R_2_PIN, HIGH);
	analogWrite(MOTOR_R_ENABLE, MOTOR_L_DUTY);
//	digitalWrite(MOTOR_R_ENABLE, HIGH);
}

// High-level functions for driving both motors at once:

void Fwd() {
	L_Fwd(); R_Fwd();
}

void Rev() {
	L_Rev(); R_Rev();
}

void Stop() {
	L_Stop(); R_Stop();
}

void Brake() {
	L_Brake(); R_Brake();
}

void veerL() {
	L_Stop(); R_Fwd();
}

void veerR() {
	L_Fwd(); R_Stop();
}

void spinL() {
	L_Rev(); R_Fwd();
}

void spinR() {
	L_Fwd(); R_Rev();
}

void low_level_test() {
	L_Fwd(); delay(2500); L_Stop(); delay(2500);
	L_Rev(); delay(2500); L_Stop(); delay(2500);
	R_Fwd(); delay(2500); L_Stop(); delay(2500);
	R_Rev(); delay(2500); L_Stop(); delay(2500);
}

void high_level_test() {
	Fwd(); delay(2500); Stop(); delay(2500);
	Rev(); delay(2500); Stop(); delay(2500);
	veerL(); delay(2500); Stop(); delay(2500);
	veerR(); delay(2500); Stop(); delay(2500);
	spinL(); delay(2500); Stop(); delay(2500);
	spinR(); delay(2500); Stop(); delay(2500);
	delay(5000);
}

void loop() {low_level_test(); high_level_test();}

void loop__() {
	L_Fwd();
	R_Fwd();
	delay(1500);
	L_Stop();
	R_Stop();
	delay(1000);
	L_Rev();
	R_Rev();
	delay(1500);
	L_Stop();
	R_Stop();
	delay(1000);
}

void loop_() {
//	digitalWrite(MOTOR_L_ENABLE, HIGH);
	analogWrite(MOTOR_L_ENABLE, 180);
	delay(2000);
	digitalWrite(MOTOR_L_ENABLE, LOW);
	delay(2000);
//	digitalWrite(MOTOR_R_ENABLE, HIGH);
	analogWrite(MOTOR_R_ENABLE, 150);
	delay(2000);
	digitalWrite(MOTOR_R_ENABLE, LOW);
	delay(2000);
}
