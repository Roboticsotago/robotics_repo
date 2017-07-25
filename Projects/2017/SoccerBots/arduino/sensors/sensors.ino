// TODO: main statement of the purpose of this code!
#include <LIS3MDL.h>
#include <Wire.h>
#include <math.h>
#include <EEPROM.h>

#define SHUTTER 0
#define DEBUGGING 1

const int NUM_SENSORS = 8;
const int analog_sensor_pins[] = {A0,A1,A2,A3,A4,A5,A6,A7};
float ir_values[NUM_SENSORS];
const float IR_GAIN_THRESHOLD = 0.5; // For culling reflections, hopefully
const int IR_THRESHOLD = 980; // About 0.15 after converting to 0..1 float looked about right, which would be ~870 raw.  In practice, with no IR ball present, we never see a raw value less than 1000
const int CALIBRATION_MODE_SWITCH_PIN = 2;
const int SAVE_HEADING_BUTTON_PIN = 7;
const int EEPROM_MIN_X = 0;
const int EEPROM_MIN_Y = 2;
const int EEPROM_MIN_Z = 4;
const int EEPROM_MAX_X = 6;
const int EEPROM_MAX_Y = 8;
const int EEPROM_MAX_Z = 10;
const int EEPROM_TARGET_HEADING = 12;

const int BUZZER = 10;
int ball_detected = 0;
float ball_angle = 0;
float ball_distance = 0;
int front_range = 0;
int back_range = 0; // so far unused
float angle_to_goal = 0;
int calibration_mode_switch = 0;
int light_sensor = 0;

#if DEBUGGING ==1
	#define DEBUG(x) Serial.println (x)
	#define DEBUG_NOEOL(x) Serial.print (x)
#else
	#define DEBUG(x)
	#define DEBUG_NOEOL(x)
#endif

const float IR_COORDINATES[NUM_SENSORS][2] = {{0.0,1.0},{0.71,0.71},{1.0,0.0},{0.71,-0.71},{0.0,-1.0},{-0.71, -0.71},{-1.0, 0.0},{-0.71, 0.71}};

#include "magnetometer.h"
#include "ultrasonic.h"

// int ir_val;
const float TAU = 2 * PI;

// To write an int to EEPROM requires two write() calls, since write() works only in bytes and an int is two.
// Not sure whether to have address be a specific byte address or to multiply by sizeof(int) within the functions to create a virtual addressing scheme for int values only.
// Probably safer to leave as raw byte addresses in case we have to mix and match, but watch for accidental overwriting!

void EEPROM_write_int(int address, int value) {
	EEPROM.write(address, highByte(value));
	EEPROM.write(address + sizeof(byte), lowByte(value));
}

// Ditto for reading:

int EEPROM_read_int(int address) {
	/*
	byte highByte = EEPROM.read(address);
	byte  lowByte = EEPROM.read(address + sizeof(byte));
	return word(highByte, lowByte);
	*/
	// Or simply:
	return word(EEPROM.read(address), EEPROM.read(address + sizeof(byte)));
}

void setup() {
	for(int n=0; n<NUM_SENSORS; n++) {
		pinMode(analog_sensor_pins[n], INPUT);
	}
	pinMode(CALIBRATION_MODE_SWITCH_PIN, INPUT_PULLUP);
	pinMode(SAVE_HEADING_BUTTON_PIN, INPUT_PULLUP);
	pinMode(BUZZER,OUTPUT);
	Serial.begin(115200);
	
	magnetometerSetup();
	ultrasonic_setup();
}

void test_loop() {
	for(int n=0; n<=NUM_SENSORS; n++) {
		DEBUG_NOEOL(analogRead(analog_sensor_pins[n]) + " ");
	}
	DEBUG();
	delay(50);
}

void loop() {
	calibration_mode_switch = digitalRead(CALIBRATION_MODE_SWITCH_PIN);
	if (!calibration_mode_switch) {
		magnetometer_calibrateMagnetometer(); 
	}
	if (!digitalRead(SAVE_HEADING_BUTTON_PIN)) { // allows the heading to be saved during use
		magnetometer_saveHeading();
	}
	readIRsensors();
	//printIRsensors();
	get_ball_angle();
	//get_calibration_mode_switch();
	angle_to_goal = magnetometer_getAngleToTarget();
	front_range = getUSDistance();
	//TODO: get_compass etc.
	send_output();
#if DEBUGGING == 1
	delay(2000);
#else 
	delay(20);
#endif
}

void beep() {
	tone(BUZZER, 3600);
	delay(100);
	noTone(BUZZER);
}

float readIRsensor(int sensor_num) { // takes single reading from one IR sensor
	int reading = analogRead(analog_sensor_pins[sensor_num]);
	DEBUG_NOEOL("sensor "); DEBUG_NOEOL(sensor_num); DEBUG_NOEOL(": "); DEBUG(reading);
	if (reading>IR_THRESHOLD) {
		return 0.0;
	} else {
		return (1023-reading)/1023.0;
	}
}

void readIRsensors() { // takes readings from all 8 sensors and stores them in an array.
	for (int i=0; i<NUM_SENSORS; i++) {
		ir_values[i] = readIRsensor(i);
	}
  printIRsensors();
  irAutoGain();
  printIRsensors();
  
}

void irAutoGain() { // stretch out the IR sensor values, apply a threshold, then scale back again, in an attempt to ignore IR reflections off the goal.
  float irMax = 0.0;
  float irMin = 1.0;
  // First, determine the min and max for the latest readings:
  for (int i=0; i<NUM_SENSORS; i++) {
    if(ir_values[i] < irMin && ir_values[i] != 0){irMin = ir_values[i];}
    if(ir_values[i] > irMax){irMax = ir_values[i];}
  }
  // Then scale to full scale, apply threshold, and scale back down:
  for(int i=0; i<NUM_SENSORS; i++){
   if((ir_values[i]-irMin)/(irMax-irMin) < IR_GAIN_THRESHOLD){
     ir_values[i]=0;
  }
  }
}

void printIRsensors() { // used for debugging
	DEBUG_NOEOL("Raw sensor readings: ");
	for (int i=0; i<NUM_SENSORS; i++) {
		DEBUG_NOEOL(ir_values[i]);
		DEBUG_NOEOL(" ");
	} 
	DEBUG();
}

void rad2degTest() {
	for (int i=0; i < 8; i ++) {
		DEBUG(i / 8.0 * TAU);
		DEBUG(normaliseDegrees(rad2deg(i / 8.0 * TAU)));
	}
}

float rad2deg(float rad) { // redundant as math.h provides degrees function
	return (rad * -1.0 + TAU / 4.0) * (360.0 / TAU);
}

float normaliseDegrees(float d) {
	//return fmod(d + 180, 360); 
	if (d < -180) {
		return d + 360;
	} else if (d>180) {
		return d - 360;
	} else {
		return d;
	}
}

float vector2distance(float vector_magnitude) { // calculates an approx. dist. to the ball from the IR array
	return exp(0.33/vector_magnitude) * 0.03;
}

void send_output() {
	Serial.print(ball_detected);Serial.print(" ");
	Serial.print(ball_angle);Serial.print(" ");
	Serial.print(ball_distance);Serial.print(" ");
	Serial.print(front_range);Serial.print(" ");
	Serial.print(back_range);Serial.print(" ");
	Serial.print(angle_to_goal);Serial.print(" ");
	Serial.print(calibration_mode_switch);Serial.print(" ");
	Serial.print(light_sensor);Serial.print(" ");
	Serial.println(";");
}

float get_ball_angle() {
	DEBUG("ball angle called!");
	
	// Calculate the centroid of the ball detection vectors...

	float x_total = 0.0; float y_total = 0.0;
	for (int n=0; n<NUM_SENSORS; n++) {
		x_total += IR_COORDINATES[n][0]*ir_values[n];
		y_total += IR_COORDINATES[n][1]*ir_values[n];

		DEBUG_NOEOL("Sensor #:");
		DEBUG_NOEOL(n);
		DEBUG_NOEOL("; value=");
		DEBUG_NOEOL(ir_values[n]);
		DEBUG_NOEOL(": (");
		DEBUG_NOEOL(IR_COORDINATES[n][0]*ir_values[n]);
		DEBUG_NOEOL(",");
		DEBUG_NOEOL(IR_COORDINATES[n][1]*ir_values[n]);
		DEBUG_NOEOL("); ");
	}

	float x_average = x_total/(float)NUM_SENSORS;
	float y_average = y_total/(float)NUM_SENSORS;
	
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
    DEBUG_NOEOL(ball_angle);
	
	// Calculate approximate distance:
	// First, determine the length of the vector (use the Pythagorean theorem):
	float vector_magnitude = sqrt(pow(x_average, 2)+pow(y_average,2));	
    if (ball_angle == 0 && vector_magnitude == 0) {
		ball_detected = 0;
	} else {
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

//TODO: define other get functions (getCompass etc)
