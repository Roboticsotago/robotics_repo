// TODO: main statement of the purpose of this code!
#include <LIS3MDL.h>
#include <Wire.h>
#include <math.h>



const int NUM_SENSORS = 8;
const int analog_sensor_pins[] = {A0,A1,A2,A3,A4,A5,A6,A7};
float ir_values[8];
float IR_COORDINATES[NUM_SENSORS][2] = {{0.0,1.0},{0.71,0.71},{1.0,0.0},{0.71,-0.71},{0.0,-1.0},{-0.71, -0.71},{-1.0, 0.0},{-0.71, 0.71}};
const int IR_THRESHOLD = 980; // About 0.15 after converting to 0..1 float looked about right, which would be ~870 raw.  In practice, with no IR ball present, we never see a raw value less than 1000.
const int CALIBRATION_MODE_SWITCH_PIN = 2;

int ball_detected = 0;
float ball_angle = 0;
float ball_distance = 0;
float front_range = 0;
float back_range = 0;
float compass_heading = 0;
int calibration_mode_switch = 0;
int light_sensor = 0;

 

#define DEBUGGING 1


#ifdef DEBUGGING 
	#define DEBUG(x) Serial.println (x)
	#define DEBUG_NOEOL(x) Serial.print (x)
#else
	#define DEBUG(x)
	#define DEBUG_NOEOL(x)
#endif

#include "magnetometer.h"

//int ir_val;
const float TAU = 2 * PI;

void setup() {
	for(int n=0; n<NUM_SENSORS; n++) {
		pinMode(analog_sensor_pins[n], INPUT);

	}
	pinMode(CALIBRATION_MODE_SWITCH_PIN, INPUT_PULLUP);
	Serial.begin(115200);
	magnetometer_setup();
}

void test_loop() {
  for(int n=0; n<=NUM_SENSORS; n++) {
    DEBUG_NOEOL(analogRead(analog_sensor_pins[n]) + " ");
  }
  DEBUG();
  delay(50);
}


void loop() {
	readIRsensors();
	printIRsensors();
	get_ball_angle();
	get_calibration_mode_switch();
	compass_heading = getRelativeAngle(getCompassHeading());
	//TODO: get_compass etc.
	send_output();
	delay(500); //TODO: reduce delay for Pd
	
}

float readIRsensor(int sensor_num){ //takes single reading from one IR sensor
	int reading = analogRead(analog_sensor_pins[sensor_num]);
	DEBUG_NOEOL("sensor "); DEBUG_NOEOL(sensor_num); DEBUG_NOEOL(": "); DEBUG(reading);
	if (reading>IR_THRESHOLD){
		return 0.0;
	}else{
		return (1023-reading)/1023.0;
	}
}

void readIRsensors(){ //takes readings from all 8 sensors and stores them in an array.
	for (int i=0; i<NUM_SENSORS; i++) {
		ir_values[i] = readIRsensor(i);
	} 
}

void printIRsensors(){ //used for debugging
	DEBUG_NOEOL("Raw sensor readings: ");
	for (int i=0; i<NUM_SENSORS; i++) {
		DEBUG_NOEOL(ir_values[i]);
		DEBUG_NOEOL(" ");
	} 
	DEBUG();
}

void rad2degTest(){
	for (int i=0; i < 8; i ++) {
		DEBUG(i / 8.0 * TAU);
		DEBUG(normaliseDegrees(rad2deg(i / 8.0 * TAU)));
	}
}

float rad2deg(float rad){ //redundant as math.h provides degrees function
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
       return exp(0.33/vector_magnitude) * 0.03;
}

void send_output(){
	Serial.print(ball_detected);Serial.print(" ");
	Serial.print(ball_angle);Serial.print(" ");
	Serial.print(ball_distance);Serial.print(" ");
	Serial.print(front_range);Serial.print(" ");
	Serial.print(back_range);Serial.print(" ");
	Serial.print(compass_heading);Serial.print(" ");
	Serial.print(calibration_mode_switch);Serial.print(" ");
	Serial.print(light_sensor);Serial.print(" ");
	Serial.println(";");
}

float get_ball_angle() {
	DEBUG("ball angle called!");
	
	// Calculate the centroid of the ball detection vectors...
	
	float x_total = 0; float y_total = 0; int count = 0; 
	for(int n=0; n<NUM_SENSORS; n++) {
		x_total += IR_COORDINATES[n][0]*ir_values[n];
		y_total += IR_COORDINATES[n][1]*ir_values[n];

		DEBUG_NOEOL(n);
		DEBUG_NOEOL(": (");
		DEBUG_NOEOL(IR_COORDINATES[n][0]*ir_values[n]);
		DEBUG_NOEOL(",");
		DEBUG_NOEOL(IR_COORDINATES[n][1]*ir_values[n]);
		DEBUG_NOEOL("); ");

		count += 1;
	}

	
	float x_average = x_total/(float)count;
	float y_average = y_total/(float)count;
	
	
	DEBUG_NOEOL("  X AVERAGE: ");
	DEBUG_NOEOL(x_average);
	DEBUG_NOEOL("  Y AVERAGE: ");
	DEBUG_NOEOL(y_average);
	DEBUG(" ");
	
	
	// Calculate angle:
	// This will use the atan2() function to determine the angle from the average x and y co-ordinates
	// You can use the degrees() function to convert for output/debugging.
	ball_angle = degrees(atan2(x_average, y_average));
    DEBUG_NOEOL("Angle to ball: ");
    DEBUG_NOEOL(degrees(ball_angle));
	
	// Calculate approximate distance:
	// First, determine the length of the vector (use the Pythagorean theorem):
	float vector_magnitude = sqrt(pow(x_average, 2)+pow(y_average,2));	
    if (ball_angle == 0 && vector_magnitude == 0){
		ball_detected = 0;
	}else{
		ball_detected = 1;
	}
	// We need to map the raw vector magnitudes to real-world distances. This is probably not linear! Will require some calibration testing...
	ball_distance = vector2distance(vector_magnitude); 
	DEBUG_NOEOL(" Vector magnitude: ");
	DEBUG_NOEOL(vector_magnitude);
	DEBUG_NOEOL(" Distance: ");
	DEBUG_NOEOL(ball_distance);
	DEBUG("");
	
	
	// TODO: maybe also check for infinity, and map that to a usable value (e.g. 0).
	
}
int get_calibration_mode_switch(){
	calibration_mode_switch = digitalRead(CALIBRATION_MODE_SWITCH_PIN); 
	if (!digitalRead(CALIBRATION_MODE_SWITCH_PIN)){
		calibrateSensor();
	}
}
//TODO: define other get functions (getCompass etc)
