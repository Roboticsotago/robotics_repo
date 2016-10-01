// Testing the IR reflectance sensors with the DSpace Robot board
// Working towards a line-following robot for Robocup 2017

// Current config (on the 2013 Dalek test-bed) uses pin 2 on each of sensor blocks SENSOR0, SENSOR1, SENSOR2, and SENSOR3.
// These correspond to A0..A3.
// A4 and A5 are also available

#define L_PIN A0
#define M_PIN A1
#define R_PIN A2

#define WHITE_ON_BLACK 1


#define MOTOR_R_ENABLE 11
#define MOTOR_L_ENABLE 5

#define MOTOR_R_1_PIN 6
#define MOTOR_R_2_PIN 7

#define MOTOR_L_1_PIN 9
#define MOTOR_L_2_PIN 10

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

const int MOTOR_L_DUTY=230;	// 180
const int MOTOR_R_DUTY=210;	// 150



// Will need light/dark thresholds for each sensor for calibration...
// ...or just one for all, if they read consistently enough
const int SENSOR_THRESHOLD=250;

// TODO: a #define flag for black line on white background or vice versa.

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

	Serial.begin(9600);
	
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



// Read and compare with threshold:

bool line_detected(int analogPin) {
	int raw_reading = analogRead(analogPin);
	// TODO: if debugging, print raw reading to serial output
	// If you need to flip the light/dark sense, do it here
#ifdef WHITE_ON_BLACK
	return (raw_reading < SENSOR_THRESHOLD);	// White line, black background
#else
	return (raw_reading > SENSOR_THRESHOLD);	// Black line, white background
#endif
}


void control() {
	bool l_line = isBlack(L_PIN);
	bool m_line = isBlack(M_PIN);
	bool r_line = isBlack(R_PIN);
	
	// TODO: could shift and combine the boolean values for more readable code
	// e.g. switch bits ... 0b000 -> lost ... 0b010 -> fwd ...
	
	if (!l_line && !m_line && !r_line) {Serial.println("Lost!"); Rev();}
	if (!l_line && !m_line &&  r_line) {Serial.println("spin right"); spinR();}
	if (!l_line &&  m_line && !r_line) {Serial.println("fwd"); Fwd();}
	if (!l_line &&  m_line &&  r_line) {Serial.println("veer right"); veerR();}
	if ( l_line && !m_line && !r_line) {Serial.println("spin left"); spinL();}
	if ( l_line && !m_line &&  r_line) {Serial.println("?!"); veerL();}
	if ( l_line &&  m_line && !r_line) {Serial.println("veer left"); veerL();}
	if ( l_line &&  m_line &&  r_line) {Serial.println("perpendicular?!"); Fwd();}
}

void debug() {
	Serial.print(line_detected(A0)); Serial.print("	");
	Serial.print(line_detected(A1)); Serial.print("	");
	Serial.print(line_detected(A2)); Serial.print("	");
	Serial.println();

	delay(100);
}

bool isBlack(int pin) { return analogRead(pin) > SENSOR_THRESHOLD;}

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
	
	delay(20);
}
