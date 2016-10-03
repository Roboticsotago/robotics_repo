// Testing the IR reflectance sensors with the DSpace Robot board
// Working towards a line-following robot for Robocup 2017

// Current config (on the 2013 Dalek test-bed) uses pin 2 on each of sensor blocks SENSOR0, SENSOR1, SENSOR2, and SENSOR3.
// These correspond to A0..A3.
// A4 and A5 are also available

// Will need light/dark thresholds for each sensor for calibration...
// ...or just one for all, if they read consistently enough
const int SENSOR_THRESHOLD=600;
#define DEBUGGING 1

// Don't go too high with these on the Dalek as it will draw too much current and cause resets!
const int MOTOR_L_DUTY=235;
const int MOTOR_R_DUTY=215;
//const int MOTOR_L_DUTY=0;	// 180
//const int MOTOR_R_DUTY=0;	// 150

const int CYCLE_TIME=2;

#define WHITE_ON_BLACK 1


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


void setup() {
	pinMode(A0, INPUT); digitalWrite(A0, LOW);
	pinMode(A1, INPUT); digitalWrite(A1, LOW);
	pinMode(A2, INPUT); digitalWrite(A2, LOW);
	
	pinMode(MOTOR_L_ENABLE, OUTPUT); digitalWrite(MOTOR_L_ENABLE, LOW);
	pinMode(MOTOR_R_ENABLE, OUTPUT); digitalWrite(MOTOR_R_ENABLE, LOW);

	pinMode(MOTOR_L_1_PIN, OUTPUT); digitalWrite(MOTOR_L_1_PIN, LOW);
	pinMode(MOTOR_L_2_PIN, OUTPUT); digitalWrite(MOTOR_L_2_PIN, HIGH);

	pinMode(MOTOR_R_1_PIN, OUTPUT); digitalWrite(MOTOR_R_1_PIN, LOW);
	pinMode(MOTOR_R_2_PIN, OUTPUT); digitalWrite(MOTOR_R_2_PIN, HIGH);

#ifdef DEBUGGING
	Serial.begin(9600);
	Serial.print("SENSOR_THRESHOLD=");
	Serial.println(SENSOR_THRESHOLD);
#endif
	
	delay(4000);
}


// Low-level functions for driving the L and R motors independently...

void L_Fwd() {
	digitalWrite(MOTOR_L_1_PIN, LOW);
	digitalWrite(MOTOR_L_2_PIN, HIGH);
	analogWrite(MOTOR_L_ENABLE, MOTOR_L_DUTY);
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
	analogWrite(MOTOR_R_ENABLE, MOTOR_R_DUTY);
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
	analogWrite(MOTOR_R_ENABLE, MOTOR_R_DUTY);
//	digitalWrite(MOTOR_R_ENABLE, HIGH);
}


void R_Drive(float speed){
	if (speed < 0){
		digitalWrite(MOTOR_R_1_PIN, HIGH);
		digitalWrite(MOTOR_R_2_PIN, LOW);
	}else{
		digitalWrite(MOTOR_R_1_PIN, LOW);
		digitalWrite(MOTOR_R_2_PIN, HIGH);
	}
	
	analogWrite(MOTOR_R_ENABLE, speed * MOTOR_R_DUTY);
}


void L_Drive(float speed){
	if (speed < 0){
		digitalWrite(MOTOR_L_1_PIN, HIGH);
		digitalWrite(MOTOR_L_2_PIN, LOW);
	}else{
		digitalWrite(MOTOR_L_1_PIN, LOW);
		digitalWrite(MOTOR_L_2_PIN, HIGH);
	}
	
	analogWrite(MOTOR_L_ENABLE, speed * MOTOR_L_DUTY);
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

void control() {
	bool l_line = isBlack(L_PIN);
	bool m_line = isBlack(M_PIN);
	bool r_line = isBlack(R_PIN);
	
	// TODO: could shift and combine the boolean values for more readable code
	// e.g. switch bits ... 0b000 -> lost ... 0b010 -> fwd ...
	
	if (!l_line && !m_line && !r_line) {Serial.println("Lost!"); Rev();}
	if (!l_line && !m_line &&  r_line) {Serial.println("spin right"); spinR();}
	if (!l_line &&  m_line && !r_line) {Serial.println("fwd"); Fwd();}
	if (!l_line &&  m_line &&  r_line) {Serial.println("veer right"); Veer(1,0.7);}
	if ( l_line && !m_line && !r_line) {Serial.println("spin left"); spinL();}
	if ( l_line && !m_line &&  r_line) {Serial.println("?!"); veerR();}
	if ( l_line &&  m_line && !r_line) {Serial.println("veer left"); Veer(0.7,1);}
	if ( l_line &&  m_line &&  r_line) {Serial.println("perpendicular?!"); Fwd();}
}

void debug() {
	Serial.print(line_detected(A0)); Serial.print("	");
	Serial.print(line_detected(A1)); Serial.print("	");
	Serial.print(line_detected(A2)); Serial.print("	");
	Serial.println();

	delay(CYCLE_TIME);
}


void loop() {
	control();
	//delay(100);
	
	//Serial.print("A0:"); Serial.print(analogRead(A0)); Serial.print("   ");
	//Serial.print("A1:"); Serial.print(analogRead(A1)); Serial.print("   ");
	//Serial.print("A2:"); Serial.println(analogRead(A2));
	
	//Serial.print(isBlack(A0)); Serial.print("   ");
	//Serial.print(isBlack(A1)); Serial.print("   ");
	//Serial.print(i475sBlack(A2)); Serial.print("   ");
	//Serial.println();
	
	//delay(CYCLE_TIME);
}

void test_loop() {
	Veer(0.7, 1);
	delay(2000);
	Veer(1, 0.7);
	delay(2000);
}
