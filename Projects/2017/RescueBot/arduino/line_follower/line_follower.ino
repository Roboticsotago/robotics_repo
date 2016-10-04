// Testing the IR reflectance sensors with the DSpace Robot board
// Working towards a line-following robot for Robocup 2017

// Current config (on the 2013 Dalek test-bed) uses pin 2 on each of sensor blocks SENSOR0, SENSOR1, SENSOR2, and SENSOR3.
// These correspond to A0..A3.
// A4 and A5 are also available

// Will need light/dark thresholds for each sensor for calibration...
// ...or just one for all, if they read consistently enough

const int SENSOR_THRESHOLD=500;
#define DEBUGGING 1
//#define DEBUG_BACKTRACKING 1

// Don't go too high with these on the Dalek as it will draw too much current and cause resets!
const int MOTOR_L_DUTY=225;
const int MOTOR_R_DUTY=210;
//const int MOTOR_L_DUTY=0;	// 180
//const int MOTOR_R_DUTY=0;	// 150

const int CYCLE_TIME=5;

//#define WHITE_ON_BLACK 1

#define L_PIN A0
#define M_PIN A1
#define R_PIN A2

// TODO: factor out these and the motor-related functions below into a library.

#define MOTOR_R_ENABLE 11
#define MOTOR_L_ENABLE 5

#define MOTOR_R_1_PIN 6
#define MOTOR_R_2_PIN 7

#define MOTOR_L_1_PIN 9
#define MOTOR_L_2_PIN 10

// Unused?
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

//const int Kp = 1;
//const int Ki = 1;
//const int Kd = 1;

// Unfortunately, there is some sharing of LED pins with motor drive still.
#define R_LED 11
#define O_LED 2
#define Y_LED 12
#define G_LED 13
#define BUZZER 3
#define L_BUTTON 19
#define R_BUTTON 8

int led_state;

int motors_enabled = 0;

int backtrack[5][10];

void setup() {
	pinMode(A0, INPUT); digitalWrite(A0, LOW);
	pinMode(A1, INPUT); digitalWrite(A1, LOW);
	pinMode(A2, INPUT); digitalWrite(A2, LOW);
	
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

#ifdef DEBUGGING
	Serial.begin(9600);
	Serial.println("\n\nDalek Prototype Line Follower v0.01");
	Serial.print("SENSOR_THRESHOLD=");
	Serial.println(SENSOR_THRESHOLD);
#endif

	// Around 2100-2500 Hz sounds good on the piezo buzzer we have, with 2100 Hz being quite loud (near resonant frequency).
	digitalWrite(G_LED, HIGH); tone(BUZZER, 2100, 100); delay(200);
	digitalWrite(G_LED, LOW);  tone(BUZZER, 2200, 100); delay(200);
	digitalWrite(G_LED, HIGH); tone(BUZZER, 2300, 100); delay(200);
	digitalWrite(G_LED, LOW);  tone(BUZZER, 2400, 100); delay(200);
	digitalWrite(G_LED, HIGH); tone(BUZZER, 2500, 100); delay(200);
	delay(200); digitalWrite(G_LED, LOW);
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
	
	analogWrite(MOTOR_L_ENABLE, speed * MOTOR_L_DUTY);
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
	
	analogWrite(MOTOR_R_ENABLE, speed * MOTOR_R_DUTY);
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

// For detecting the line, we'll have a function to read an analog value for each sensor and compare with a light/dark threshold.

// Here's a nice compact black line detector one-liner:
// bool isBlack(int pin) { return analogRead(pin) > SENSOR_THRESHOLD;}

// For debugging, we might want to be able to see what's going on at the analogRead() level:

bool isBlack(int pin) {
	int raw_reading = analogRead(pin);
#ifdef DEBUGGING
	Serial.print("pin=");
	Serial.print(pin);
	Serial.print(" val=");
	Serial.print(raw_reading);
	Serial.print("	");
//	Serial.println();
#endif
#ifdef WHITE_ON_BLACK
	return (raw_reading < SENSOR_THRESHOLD);	// White line, black background
#else
	return (raw_reading > SENSOR_THRESHOLD);	// Black line, white background
#endif
}


// TODO: implement some sort of memory of recent moves, so we can do more meaningful corrections if we get lost, etc.

// We'll store the line detection readings globally for easier debugging:
bool l_line;
bool m_line;
bool r_line;

void debug_line_sensors_with_LEDs() {
	// TODO: invert when in white-on-black mode?
	digitalWrite(O_LED, l_line);
	digitalWrite(Y_LED, m_line);
	digitalWrite(G_LED, r_line);
}

void control() {
//	digitalWrite(Y_LED, LOW);
	
	l_line = isBlack(L_PIN);
	m_line = isBlack(M_PIN);
	r_line = isBlack(R_PIN);
	
	debug_line_sensors_with_LEDs();
	
	// TODO: could shift and combine the boolean values for more readable code
	// e.g. switch bits ... 0b000 -> lost ... 0b010 -> fwd ...
	
	if (!l_line && !m_line && !r_line) {
		Serial.println("Lost!");
		// Would be nice to light red LED if lost.
	//	digitalWrite(Y_LED, HIGH);
		Fwd();
	//	retrace();
	}
	if (!l_line && !m_line &&  r_line) {
		Serial.println("spin right"); 
		spinR();
	//	calc_track(0);
	}
	if (!l_line &&  m_line && !r_line) {
		Serial.println("fwd"); 
		Fwd();
	//	calc_track(2);
	}
	if (!l_line &&  m_line &&  r_line) {
-		Serial.println("veer right"); 
		Veer(1,0.7);
	//	calc_track(1);
		}
	if ( l_line && !m_line && !r_line) {
		Serial.println("spin left"); 
		spinL();
	//	calc_track(4);
	}
	if ( l_line && !m_line &&  r_line) {
		Serial.println("?!"); 
	//	digitalWrite(Y_LED, HIGH);
		veerR();
	//	calc_track(1);
	}
	if ( l_line &&  m_line && !r_line) {
		Serial.println("veer left"); 
		Veer(0.7,1);
	//	calc_track(3);
	}
	if ( l_line &&  m_line &&  r_line) {
		Serial.println("perpendicular?!"); 
	//	digitalWrite(Y_LED, HIGH);
		Fwd();
	//	calc_track(2);
	}
}

void debug() {
	Serial.print(isBlack(A0)); Serial.print("	");
	Serial.print(isBlack(A1)); Serial.print("	");
	Serial.print(isBlack(A2)); Serial.print("	");
	Serial.println();

	delay(CYCLE_TIME);
}

void toggleLED(){
	led_state = !led_state;
	digitalWrite(G_LED, led_state);
}

void loop() {
	if (!digitalRead(R_BUTTON)) {motors_enabled = 1;}
	if (!digitalRead(L_BUTTON)) {motors_enabled = 0;}
	if (!motors_enabled) {Stop();}
	
	control();
	
#ifdef DEBUG_BACKTRACKING
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Serial.print(backtrack[i][j]);
			Serial.print(" ");
		}
		Serial.println();
	}
	//delay(300);
#endif

//	toggleLED();

	delay(CYCLE_TIME);

	
	//Serial.print("A0:"); Serial.print(analogRead(A0)); Serial.print("   ");
	//Serial.print("A1:"); Serial.print(analogRead(A1)); Serial.print("   ");
	//Serial.print("A2:"); Serial.println(analogRead(A2));
	
	//Serial.print(isBlack(A0)); Serial.print("   ");
	//Serial.print(isBlack(A1)); Serial.print("   ");
	//Serial.print(i475sBlack(A2)); Serial.print("   ");
	//Serial.println();
	
	//delay(CYCLE_TIME);
}

void loop_test() {
	digitalWrite(O_LED, HIGH);
	delay(200);
	digitalWrite(O_LED, LOW);
	delay(200);
	digitalWrite(Y_LED, HIGH);
	delay(200);
	digitalWrite(Y_LED, LOW);
	delay(200);
	digitalWrite(R_LED, HIGH);
	delay(200);
	digitalWrite(R_LED, LOW);
	delay(200);
	digitalWrite(G_LED, HIGH);
	delay(200);
	digitalWrite(G_LED, LOW);
	delay(200);
	delay(2000);
	
}
