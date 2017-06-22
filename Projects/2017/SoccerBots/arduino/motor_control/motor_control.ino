//Currently working Motor Control using the DSpace robot board for RoboCup Soccer 2017.

#define BAUD_RATE 115200 

//TODO: Do the duty cycles of the SoccerBot motors need adjusting? 
#include <Servo.h> //incldued for kicker
Servo Kicker;
const int SERVO_PIN = 13; //Servo 2, Pin 2 SDK.
const int KICKER_MIN = 100;
const int KICKER_MAX = 60; //tested 
const int KICKER_MID = 80;
const int KICKER_DELAY = 1000;
const int MOTOR_L_DUTY=128; //to limit 8.0V to 4.5V
const int MOTOR_R_DUTY=128;
const int DIR_MASK 		= 0b00100000;
const int MOTOR_MASK 		= 0b01000000;
const int SPEED_MASK	 	= 0b00011111;
const int MESSAGE_TYPE_MASK     = 0b10000000;
const int KICKER_MASK 		= 0b00000001;
const int MOTOR_TOGGLE_SWITCH = 18; //physical pin 2 on sensor block 4.
int motors_enabled = 0;

#define MOTOR_R_ENABLE 5
#define MOTOR_L_ENABLE 11

#define MOTOR_R_1_PIN 10
#define MOTOR_R_2_PIN 9

#define MOTOR_L_1_PIN 7
#define MOTOR_L_2_PIN 6

// Unused?
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

#define R_LED 11
#define O_LED 2
#define Y_LED 12
#define G_LED 13
#define BUZZER 3
#define L_BUTTON 19
#define R_BUTTON 8

#define DEBUGGING 0
/*
#define debug(message) \
	do { if (DEBUGGING) Serial.println(message); } while (0)
*/
#ifdef DEBUGGING //Do we need to print some values all the time? Pure data isn't expecting anything in return to the motor driving protocol.
	#define DEBUG(x) Serial.println (x)
	#define DEBUG_NOEOL(x) Serial.print (x)
#else
	#define DEBUG(x)
	#define DEBUG_NOEOL(x)
#endif




void setup() {
	Kicker.attach(SERVO_PIN);
	Kicker.write(KICKER_MIN);
	pinMode(MOTOR_TOGGLE_SWITCH, INPUT); digitalWrite(MOTOR_TOGGLE_SWITCH, 1);
	pinMode(R_LED, OUTPUT); digitalWrite(R_LED, LOW);
	pinMode(O_LED, OUTPUT); digitalWrite(O_LED, LOW);
	pinMode(Y_LED, OUTPUT); digitalWrite(Y_LED, LOW);
	pinMode(G_LED, OUTPUT); digitalWrite(G_LED, LOW);
	pinMode(BUZZER, OUTPUT);
	pinMode(L_BUTTON, INPUT); digitalWrite(L_BUTTON, HIGH); // or INPUT_PULLUP on newer Arduino
	pinMode(R_BUTTON, INPUT); digitalWrite(R_BUTTON, HIGH);	// NOTE: hardware problem with Sensor 0 Pin 1 on the Dalek board?  It's stuck at only about 1.7 V when pulled high.  Oh, hardwired onboard LED!  Have now removed resistor R4 to open that circuit. :)

	pinMode(MOTOR_L_ENABLE, OUTPUT); digitalWrite(MOTOR_L_ENABLE, LOW);
	pinMode(MOTOR_R_ENABLE, OUTPUT); digitalWrite(MOTOR_R_ENABLE, LOW);

	pinMode(MOTOR_L_1_PIN, OUTPUT); digitalWrite(MOTOR_L_1_PIN, LOW);
	pinMode(MOTOR_L_2_PIN, OUTPUT); digitalWrite(MOTOR_L_2_PIN, HIGH);

	pinMode(MOTOR_R_1_PIN, OUTPUT); digitalWrite(MOTOR_R_1_PIN, LOW);
	pinMode(MOTOR_R_2_PIN, OUTPUT); digitalWrite(MOTOR_R_2_PIN, HIGH);

	Serial.begin(BAUD_RATE);

	#ifdef DEBUGGING
	DEBUG("\n\nDspace Motor Controller for SoccerBots - Info Sci Mechatronics v0.01");
	#endif
}

// Low-level functions for driving the L and R motors independently...
void L_Fwd() {
	if (!motors_enabled) return;
	digitalWrite(MOTOR_L_1_PIN, LOW);
	digitalWrite(MOTOR_L_2_PIN, HIGH);
	analogWrite(MOTOR_L_ENABLE, MOTOR_L_DUTY);
//	digitalWrite(MOTOR_L_ENABLE, HIGH);
}

void L_Rev() {
	if (!motors_enabled) return;
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
	if (!motors_enabled) return;
	digitalWrite(MOTOR_L_1_PIN, HIGH);
	digitalWrite(MOTOR_L_2_PIN, HIGH);
	analogWrite(MOTOR_L_ENABLE, MOTOR_L_DUTY);
//	digitalWrite(MOTOR_L_ENABLE, HIGH);
}


void R_Fwd() {
	if (!motors_enabled) return;
	digitalWrite(MOTOR_R_1_PIN, LOW);
	digitalWrite(MOTOR_R_2_PIN, HIGH);
	analogWrite(MOTOR_R_ENABLE, MOTOR_R_DUTY);
//	digitalWrite(MOTOR_R_ENABLE, HIGH);
}

void R_Rev() {
	if (!motors_enabled) return;
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
	if (!motors_enabled) return;
	digitalWrite(MOTOR_R_1_PIN, HIGH);
	digitalWrite(MOTOR_R_2_PIN, HIGH);
	analogWrite(MOTOR_R_ENABLE, MOTOR_R_DUTY);
//	digitalWrite(MOTOR_R_ENABLE, HIGH);
}


void L_Drive(float speed){
	if (!motors_enabled) return;
	if (speed < 0){
		digitalWrite(MOTOR_L_1_PIN, HIGH);
		digitalWrite(MOTOR_L_2_PIN, LOW);
	}else{
		digitalWrite(MOTOR_L_1_PIN, LOW);
		digitalWrite(MOTOR_L_2_PIN, HIGH);
	}

	analogWrite(MOTOR_L_ENABLE, (int) round(speed * MOTOR_L_DUTY));
}


void R_Drive(float speed){
	if (!motors_enabled) return;
	if (speed < 0){
		digitalWrite(MOTOR_R_1_PIN, HIGH);
		digitalWrite(MOTOR_R_2_PIN, LOW);
	}else{
		digitalWrite(MOTOR_R_1_PIN, LOW);
		digitalWrite(MOTOR_R_2_PIN, HIGH);
	}

	analogWrite(MOTOR_R_ENABLE, (int) round(speed * MOTOR_R_DUTY));
}


// High-level functions for driving both motors at once:

void Veer(float left_speed, float right_speed){
	L_Drive(left_speed); R_Drive(right_speed);

}

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

void L_Spd(int speed, bool dir) {
	if (!motors_enabled) {speed = 0;}
	digitalWrite(MOTOR_L_1_PIN, dir);
	digitalWrite(MOTOR_L_2_PIN, !dir);
	analogWrite(MOTOR_L_ENABLE, speed);
}


void R_Spd(int speed, bool dir) {
	if (!motors_enabled) {speed = 0;}
	digitalWrite(MOTOR_R_1_PIN, dir);
	digitalWrite(MOTOR_R_2_PIN, !dir);
	analogWrite(MOTOR_R_ENABLE, speed);
}

void kick(){ //this is unused, but it could be useful so it can stay. 
	Kicker.write(KICKER_MAX);
	delay(KICKER_DELAY);
	Kicker.write(KICKER_MIN);
	delay(KICKER_DELAY);

}

void kicker_move(int direction) {
	if (!motors_enabled) {kicker_midpoint();} //will this work better?
	Kicker.write(direction? KICKER_MAX: KICKER_MIN);
}

void motor_control(){
	if (Serial.available() > 0) {
		int data = Serial.read();
		if ((data&MESSAGE_TYPE_MASK)>>7==0){
			kicker_move(data&KICKER_MASK);
        }else{
			if ((data&MOTOR_MASK)>>6) {
				R_Spd((data&SPEED_MASK)<<3, (data&DIR_MASK)>>5);
				DEBUG("R forward (DIR, Speed): ");
				DEBUG((data&DIR_MASK)>>5);
				DEBUG((data&SPEED_MASK)<<3);
			} else {
				L_Spd((data&SPEED_MASK)<<3, (data&DIR_MASK)>>5);
				DEBUG("L forward (DIR, Speed): ");
				DEBUG((data&DIR_MASK)>>5);
				DEBUG((data&SPEED_MASK)<<3);
			}
		}
	} else {
		if (!motors_enabled) {Stop();}
	}
}


void kicker_midpoint(){
  Kicker.write(KICKER_MID);	
}

void loop(){
	motors_enabled = !digitalRead(MOTOR_TOGGLE_SWITCH);
	motor_control();
}


void motor_test() {
  motors_enabled = !digitalRead(MOTOR_TOGGLE_SWITCH);
  DEBUG(motors_enabled);
  Fwd();
  delay(1000);
  Stop();
  kicker_move(1);

  delay(500);

  Rev();
  delay(1000);
  Stop();
  kicker_move(0);

  delay(500);
}
