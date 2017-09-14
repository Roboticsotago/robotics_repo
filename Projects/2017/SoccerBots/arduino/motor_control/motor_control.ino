//Currently working Motor Control using the DSpace robot board for RoboCup Soccer 2017.

// MOST IMPORTANT!!! Make sure this is set correctly before uploading:
#define SHUTTER 1
// (it might be nice to have the robot's identity stored in EEPROM and detected automatically by this program)


// Stuff below here shouldn't need to change very often.

#define BAUD_RATE 9600

// TODO: Do the duty cycles of the SoccerBot motors need adjusting?
// TODO: Move the duty cycles into the eeprom
// TODO: Remove unused stuff copied from the Dalek program.

#include <Servo.h> //incldued for kicker

Servo Kicker;
const int SERVO_PIN = 13; //Servo 2, Pin 2 SDK.
const int KICKER_DELAY = 1000;

// For PWM limiting output power to 4.5 V equivalent from an 8 V supply:
// duty = 4.5 V / (8 V) x 255 = 143

#if (SHUTTER==1)
// Motor speed and servo limits tweaked for Shutter (attacker):
const int MOTOR_L_DUTY=123;
const int MOTOR_R_DUTY=130;
const int KICKER_MIN = 105;  //tested
const int KICKER_MAX = 145;
#else
// Motor speed and servo limits tweaked for Boris (goalie):
const int MOTOR_L_DUTY=134;
const int MOTOR_R_DUTY=143;
const int KICKER_MIN = 115;  //tested
const int KICKER_MAX = 60;   //tested
#endif

const int MESSAGE_TYPE_MASK	= 0b10000000;
const int MOTOR_MASK 		= 0b01000000;
const int DIR_MASK 			= 0b00100000;
const int SPEED_MASK	 	= 0b00011111;
const int KICKER_MASK 		= 0b00000001;
const int MOTOR_TOGGLE_SWITCH = 18; //physical pin 2 on sensor block 2.

int motors_enabled = 0;

#define MOTOR_R_ENABLE 5
#define MOTOR_L_ENABLE 11

#define MOTOR_R_1_PIN 10
#define MOTOR_R_2_PIN 9

#define MOTOR_L_1_PIN 7
#define MOTOR_L_2_PIN 6


#define DEBUGGING 0
/*
#define debug(message) \
	do { if (DEBUGGING) Serial.println(message); } while (0)
*/
#if DEBUGGING == 1 //Do we need to print some values all the time? Pure data isn't expecting anything in return to the motor driving protocol.
	#define DEBUG(x) Serial.println (x)
	#define DEBUG_NOEOL(x) Serial.print (x)
#else
	#define DEBUG(x)
	#define DEBUG_NOEOL(x)
#endif


void setup() {
	Kicker.attach(SERVO_PIN);
	pinMode(MOTOR_TOGGLE_SWITCH, INPUT); digitalWrite(MOTOR_TOGGLE_SWITCH, 1);

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

	// Flap kicker paddle to signal startup (even if motors disabled):
	Kicker.write(KICKER_MAX); delay(100);
	Kicker.write(KICKER_MIN); delay(100);
	Kicker.write(KICKER_MAX); delay(100);
	Kicker.write(KICKER_MIN); delay(100);
	Kicker.write(KICKER_MAX); delay(100);
	Kicker.write(KICKER_MIN);
}

void Stop() {
	digitalWrite(MOTOR_L_1_PIN, LOW);
	digitalWrite(MOTOR_L_2_PIN, HIGH);
	analogWrite(MOTOR_L_ENABLE, 0);
	digitalWrite(MOTOR_R_1_PIN, LOW);
	digitalWrite(MOTOR_R_2_PIN, HIGH);
	analogWrite(MOTOR_R_ENABLE, 0);
}

//speed argument expected to be between 0-255
void L_Spd(int speed, bool dir) {
	if (!motors_enabled) {speed = 0;}
	if (speed < 0) {speed = 0;}
	if (speed > 255) {speed = 255;}
	digitalWrite(MOTOR_L_1_PIN, dir);
	digitalWrite(MOTOR_L_2_PIN, !dir);
	analogWrite(MOTOR_L_ENABLE, (int) round(speed/255.0 * MOTOR_L_DUTY));
}

void R_Spd(int speed, bool dir) {
	if (!motors_enabled) {speed = 0;}
	if (speed < 0) {speed = 0;}
	if (speed > 255) {speed = 255;}
	digitalWrite(MOTOR_R_1_PIN, dir);
	digitalWrite(MOTOR_R_2_PIN, !dir);
	analogWrite(MOTOR_R_ENABLE, (int) round(speed/255.0 * MOTOR_R_DUTY));
}

void kick(){ //this is unused, but it could be useful so it can stay. 
	Kicker.write(KICKER_MAX);
	delay(KICKER_DELAY);
	Kicker.write(KICKER_MIN);
	delay(KICKER_DELAY);
}

void kicker_move(int direction) {
	if (!motors_enabled) {
		Kicker.write(KICKER_MIN); 
	} else{
		Kicker.write(direction ? KICKER_MAX : KICKER_MIN);
	}
}

void motor_control(){
	if (Serial.available() > 0) {
		int data_int = Serial.read();
		if (data_int < 0) {
			return;
		}
		byte data = byte(data_int);
		if ((data&MESSAGE_TYPE_MASK)>>7==0){
			kicker_move(data&KICKER_MASK);
		} else {
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

void loop(){
	motors_enabled = !digitalRead(MOTOR_TOGGLE_SWITCH);
	motor_control();
}

