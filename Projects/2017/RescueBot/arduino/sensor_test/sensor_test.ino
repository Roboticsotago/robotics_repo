// Testing the IR reflectance sensors with the DSpace Robot board
// Working towards a line-following robot for Robocup 2017

// Current config (on the 2013 Dalek test-bed) uses pin 2 on each of sensor blocks SENSOR0, SENSOR1, SENSOR2, and SENSOR3.
// These correspond to A0..A3.
// A4 and A5 are also available

#define L_PIN A0
#define M_PIN A1
#define R_PIN A2

#define WHITE_ON_BLACK 1


// Will need light/dark thresholds for each sensor for calibration...
// ...or just one for all, if they read consistently enough
const int SENSOR_THRESHOLD=250;

// TODO: a #define flag for black line on white background or vice versa.

void setup() {
	pinMode(A0, INPUT); digitalWrite(A0, LOW);
	pinMode(A1, INPUT); digitalWrite(A1, LOW);
	pinMode(A2, INPUT); digitalWrite(A2, LOW);
	Serial.begin(9600);
	
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
	
	if (!l_line && !m_line && !r_line) {Serial.println("Lost!");}
	if (!l_line && !m_line &&  r_line) {Serial.println("spin right");}
	if (!l_line &&  m_line && !r_line) {Serial.println("fwd");}
	if (!l_line &&  m_line &&  r_line) {Serial.println("veer right");}
	if ( l_line && !m_line && !r_line) {Serial.println("spin left");}
	if ( l_line && !m_line &&  r_line) {Serial.println("?!");}
	if ( l_line &&  m_line && !r_line) {Serial.println("veer left");}
	if ( l_line &&  m_line &&  r_line) {Serial.println("perpendicular?!");}
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
	
	delay(250);
}
