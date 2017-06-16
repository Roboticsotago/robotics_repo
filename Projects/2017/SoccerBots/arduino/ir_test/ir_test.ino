// TODO: main statement of the purpose of this code!

#include <math.h>

const int IR_1 = 2;
const int IR_2 = 3;
const int IR_3 = 4;
const int IR_4 = 5;
const int NUM_SENSORS = 8;
//const int sensor_pins[] = {2,3,4,5,6,7,8,9};
const int analog_sensor_pins[] = {A0,A1,A2,A3,A4,A5,A6,A7};
float ir_values[8];
float IR_COORDINATES[NUM_SENSORS][2] = {{0.0,1.0},{0.71,0.71},{1.0,0.0},{0.71,-0.71},{0.0,-1.0},{-0.71, -0.71},{-1.0, 0.0},{-0.71, 0.71}};
const int IR_THRESHOLD = 980; // About 0.15 after converting to 0..1 float looked about right, which would be ~870 raw.  In practice, with no IR ball present, we never see a raw value less than 1000.

int ir_val;
const float TAU = 2 * PI;

void setup() {
	for(int n=0; n<NUM_SENSORS; n++) {
		//pinMode(sensor_pins[n], INPUT);
                pinMode(analog_sensor_pins[n], INPUT);

	}
	Serial.begin(115200);
	//Serial.println("Welcome Uno");
}

void test_loop() {
  for(int n=0; n<=NUM_SENSORS; n++) {
    Serial.print(analogRead(analog_sensor_pins[n]) + " ");
  }
  Serial.println();
  delay(50);
}


void loop() {
	readIRsensors();
	//printIRsensors();
	ball_angle();
	delay(500);
}

float readIRsensor(int sensor_num){
	int reading = analogRead(analog_sensor_pins[sensor_num]);
	// TODO: debugging
	//Serial.print("sensor "); Serial.print(sensor_num); Serial.print(": "); Serial.println(reading);
	if (reading>IR_THRESHOLD){
		return 0.0;
	}else{
		return (1023-reading)/1023.0;
	}
}

void readIRsensors(){
	for (int i=0; i<NUM_SENSORS; i++) {
		ir_values[i] = readIRsensor(i);
	} 
}

void printIRsensors(){
	Serial.print("Raw sensor readings: ");
	for (int i=0; i<NUM_SENSORS; i++) {
		Serial.print(ir_values[i]);
		Serial.print(" ");
	} 
	Serial.println();
}

void rad2degTest(){
	for (int i=0; i < 8; i ++) {
		Serial.println(i / 8.0 * TAU);
		Serial.println(normaliseDegrees(rad2deg(i / 8.0 * TAU)));
	}
}

float rad2deg(float rad){
	return (rad * -1.0 + TAU / 4.0) * (360.0 / TAU);
}

float normaliseDegrees(float d){
	//return fmod(d + 180, 360); 
	if (d < -180){
		return d + 360;
	}else if(d>180){
		return d - 360;
	}else{
		return d;
	}
}
float vector2distance(float vector_magnitude){
       return exp(-27.3408*vector_magnitude + 2.88248);
}
float ball_angle() {
	//Serial.println("ball angle called!");
	
	// Calculate the centroid of the ball detection vectors...
	
	float x_total = 0; float y_total = 0; int count = 0; 
	for(int n=0; n<NUM_SENSORS; n++) {
		x_total += IR_COORDINATES[n][0]*ir_values[n];
		y_total += IR_COORDINATES[n][1]*ir_values[n];
		/*
		Serial.print(n);
		Serial.print(": (");
		Serial.print(IR_COORDINATES[n][0]*ir_values[n]);
		Serial.print(",");
		Serial.print(IR_COORDINATES[n][1]*ir_values[n]);
		Serial.print("); ");
		*/
		count += 1;
	}
	//Serial.println();
	
	float x_average = x_total/(float)count;
	float y_average = y_total/(float)count;
	
	/*
	Serial.print("  X AVERAGE: ");
	Serial.print(x_average);
	Serial.print("  Y AVERAGE: ");
	Serial.print(y_average);
	Serial.println(" ");
	*/
	
	// Calculate angle:
	// This will use the atan2() function to determine the angle from the average x and y co-ordinates
	// You can use the degrees() function to convert for output/debugging.
	float angle = atan2(x_average, y_average);
        Serial.print("Angle to ball: ");
        Serial.print(degrees(angle));
	
	// Calculate approximate distance:
	// First, determine the length of the vector (use the Pythagorean theorem):
	float vector_magnitude = sqrt(pow(x_average, 2)+pow(y_average,2));	// TODO: your code here
        
	// We need to map the raw vector magnitudes to real-world distances. This is probably not linear! Will require some calibration testing...
	float distance = vector2distance(vector_magnitude); // TODO: your code here
	Serial.print(" Vector magnitude: ");
	Serial.print(vector_magnitude);
	Serial.print(" Distance: ");
	Serial.print(distance);
	Serial.println("");
	// TODO: maybe also check for infinity, and map that to a usable value (e.g. 0).
}
