// Testing the IR reflectance sensors with the DSpace Robot board
// Working towards a line-following robot for Robocup 2017

// Current config (on the 2013 Dalek test-bed) uses pin 2 on each of sensor blocks SENSOR0, SENSOR1, SENSOR2, and SENSOR3.
// These correspond to A0..A3.
// A4 and A5 are also available

// Will need light/dark thresholds for each sensor for calibration...
// ...or just one for all, if they read consistently enough


#define BAUD_RATE 115200

// TODO: this was true of the Dalek, but what about the SoccerBot?:
// Right Dalek motor is considerably more powerful, so we need to be able to regulate each separately...
// Don't go too high with these on the Dalek as it will draw too much current and cause resets!
#include <Servo.h> //incldued for kicker
Servo Kicker;
const int SERVO_PIN = 13; //Servo 2, Pin 2 SDK.
const int KICKER_MIN = 100;
const int KICKER_MAX = 70; //these will need testing.
const int KICKER_DELAY = 1000;
const int MOTOR_L_DUTY=128;
const int MOTOR_R_DUTY=128;
const int DIR_MASK 		= 0b00100000;
const int MOTOR_MASK 	= 0b01000000;
const int SPEED_MASK 	= 0b00011111;
const int KICKER_MASK 	= 0b10000000;

#define MOTOR_R_ENABLE 11
#define MOTOR_L_ENABLE 5

#define MOTOR_R_1_PIN 6
#define MOTOR_R_2_PIN 7

#define MOTOR_L_1_PIN 9
#define MOTOR_L_2_PIN 10

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
#ifdef DEBUGGING
	#define DEBUG(x) Serial.println (x)
#else
	#define DEBUG(x)
#endif


const int MOTOR_TOGGLE_SWITCH = 18; //physical pin 2 on sensor block 4.

int motors_enabled = 0;

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
	Serial.println("\n\nDspace Motor Controller - Info Sci Mechatronics v0.01");
#endif
}

// TODO: we don't have a buzzer on the SoccerBot, so this can go:

	// Around 2100-2500 Hz sounds good on the piezo buzzer we have, with 2100 Hz being quite loud (near resonant frequency).
/*	digitalWrite(G_LED, HIGH); tone(BUZZER, 2100, 100); delay(200);
	digitalWrite(G_LED, LOW);  tone(BUZZER, 2200, 100); delay(200);
	digitalWrite(G_LED, HIGH); tone(BUZZER, 2300, 100); delay(200);
	digitalWrite(G_LED, LOW);  tone(BUZZER, 2400, 100); delay(200);
	digitalWrite(G_LED, HIGH); tone(BUZZER, 2500, 100); delay(200);
	delay(200); digitalWrite(G_LED, LOW);
}

// Audible click for debugging
// WARNING: using click() as defined below makes the right motor not work at all.  Shared pins, maybe?  Perhaps it needs a delay() of at least the duration of the click?
/*
void click() {
	tone(BUZZER, 2100, 2);
}


void beep_bad() {
	tone(BUZZER, 2500, 100); delay(200);
	tone(BUZZER, 2100, 200); delay(200);
}

void beep_good() {
	tone(BUZZER, 2100, 100); delay(100);
	tone(BUZZER, 2500, 100); delay(100);
}
*/
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
	digitalWrite(MOTOR_L_1_PIN, dir);
	analogWrite(MOTOR_L_ENABLE, speed);
}

void R_Spd(int speed, bool dir) {
	digitalWrite(MOTOR_R_1_PIN, dir);
	analogWrite(MOTOR_R_ENABLE, speed);
}

void kick(){
	Kicker.write(KICKER_MAX);
	delay(KICKER_DELAY);
	Kicker.write(KICKER_MIN);
	delay(KICKER_DELAY);

}

void kicker_move(int direction) {
	Kicker.write(direction? KICKER_MAX: KICKER_MIN);
}

/*
void calc_track(int move)
{
	if (backtrack[move][0] > 0)
	{
		backtrack[move][0]++;
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 9; j > 0; j--)
			{
				backtrack[i][j] = backtrack[i][j-1];
			}
		}
		for (int i = 0; i < 5; i++)
		{
			backtrack[i][0] = 0;
		}
		backtrack[move][0] = 1;
	}
}


void retrace()
{
	for (int i = 0; i < 10; i++)
	{
		if (backtrack[i][0] > 0)
		{
			//spin right, so spin left
			for (int j = 0; j < backtrack[i][0]; j++)
				spinL();
		}
		if (backtrack[i][1] > 0)
		{
			//veer right, so veer left
			for (int j = 0; j < backtrack[i][1]; j++)
				Veer(0.7,1);
		}
		if (backtrack[i][2] > 0)
		{
			//forward, so move back
			for (int j = 0; j < backtrack[i][2]; j++)
				Rev();
		}
		if (backtrack[i][3] > 0)
		{
			//veer left, so veer right
			for (int j = 0; j < backtrack[i][3]; j++)
				Veer(1,0.7);
		}
		if (backtrack[i][4] > 0)
		{
			//spin left, so spin right
			for (int j = 0; j < backtrack[i][4]; j++)
				spinR();
		}
	}

}
*/
// For detecting the line, we'll have a function to read an analog value for each sensor and compare with a light/dark threshold.




/*
void control() {
//	digitalWrite(Y_LED, LOW);

	l_line = isBlack(L_PIN);
	m_line = isBlack(M_PIN);
	r_line = isBlack(R_PIN);

	debug_line_sensors_with_LEDs();

	// TODO: could shift and combine the boolean values for more readable code
	// e.g. switch bits ... 0b000 -> lost ... 0b010 -> fwd ...

	if (!l_line && !m_line && !r_line) {
		DEBUG("Lost!");
		// Would be nice to light red LED if lost.
	//	digitalWrite(Y_LED, HIGH);
	//	beep_bad();
	//	Fwd();
		Rev();
	//	Veer(-0.9,-0.7);	// TODO: figure out why the left motor doesn't work here.
	//	retrace();
	}
	if (!l_line && !m_line &&  r_line) {
		DEBUG("spin right");
		spinR();
	//	calc_track(0);
	}
	if (!l_line &&  m_line && !r_line) {
		DEBUG("fwd");
	//	beep_good();
		Fwd();
	//	calc_track(2);
	}
	if (!l_line &&  m_line &&  r_line) {
		DEBUG("veer right");
		Veer(1.0,0.8);
	//	calc_track(1);
		}
	if ( l_line && !m_line && !r_line) {
		DEBUG("spin left");
		spinL();
	//	calc_track(4);
	}
	if ( l_line && !m_line &&  r_line) {
		DEBUG("?!");
	//	digitalWrite(Y_LED, HIGH);
		Stop();
	//	calc_track(1);
	}
	if ( l_line &&  m_line && !r_line) {
		DEBUG("veer left");
		Veer(0.8,1.0);
	//	calc_track(3);
	}
	if ( l_line &&  m_line &&  r_line) {
		DEBUG("perpendicular?!");
	//	digitalWrite(Y_LED, HIGH);
	//	Fwd();
		Veer(1.0,0.6);
	//	calc_track(2);
	}
}


void toggleLED(){
	led_state = !led_state;
	digitalWrite(G_LED, led_state);
}
*/

/*void motor_control(){
	if(Serial.available() > 0){
		int data = Serial.read();
		if((data&MOTOR_MASK)>>7 == 1){
			R_Spd((data&SPEED_MASK)<<2, (data&DIR_MASK)>>6);
			Serial.println("R forward");
			Serial.println((data&SPEED_MASK)<<2);
		}
		else{
			L_Spd((data&SPEED_MASK)<<2, (data&DIR_MASK)>>6);
			Serial.println("L forward");
			Serial.println((data&SPEED_MASK)<<2);
		}

	}
}

void test_loop() {
	//TODO: Add servo control code.
	if (!digitalRead(MOTOR_TOGGLE_SWITCH)) {motors_enabled = 1;}
	if (digitalRead(MOTOR_TOGGLE_SWITCH)) {motors_enabled = 0;}
	if (!motors_enabled) {Stop();}else(){motor_control();}

	//delay(CYCLE_TIME);

}
 */

void motor_control(){
	if (Serial.available() > 0) {
		int data = Serial.read();
		kicker_move((data&KICKER_MASK)>>7);
		if ((data&MOTOR_MASK)>>6) {
			R_Spd((data&SPEED_MASK)<<3, (data&DIR_MASK)>>5);
			Serial.println("R forward");
			Serial.println((data&SPEED_MASK)<<3);
		} else {
			L_Spd((data&SPEED_MASK)<<3, (data&DIR_MASK)>>5);
			Serial.println("L forward");
			Serial.println((data&SPEED_MASK)<<3);
		}
	} else {
		if (!motors_enabled) {Stop();}
	}
}

void servo_midpoint(){
  Kicker.write(95);	// TODO: fix hardcoding
}

void loop(){
	motors_enabled = !digitalRead(MOTOR_TOGGLE_SWITCH);
	motor_control();
}

void _loop() {
  motors_enabled = !digitalRead(MOTOR_TOGGLE_SWITCH);
  Serial.println(motors_enabled);
  Fwd();
  delay(1000);
  Stop();
  kick();

  delay(500);

  Rev();
  delay(1000);
  Stop();
  kick();

  delay(500);
}
