// TODO: main statement of the purpose of this code!
#include <LIS3MDL.h>
#include <Wire.h>
#include <math.h>
#include <EEPROM.h>
#include <HTInfraredSeeker.h>


#define SHUTTER 1
#define DEBUGGING 0

const int NUM_SENSORS = 8;
const int analog_sensor_pins[] = {A0,A1,A2,A3,A4,A5,A6,A7};
const int ir_sensor_angles[] = {180, -107, -80, -53, -27, 0, 27, 53, 80, 107};
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
float ball_distance = 1;
int front_range = 0;
int back_range = 0; // so far unused
float angle_to_goal = 0;
int calibration_mode_switch = 0;
int light_sensor = 0;
int crotchet;

#if DEBUGGING ==1
	#define DEBUG(x) Serial.println (x)
	#define DEBUG_NOEOL(x) Serial.print (x)
#else
	#define DEBUG(x)
	#define DEBUG_NOEOL(x)
#endif



#include "nokia.h"
#include "mk_shortened.h"
#include "sherlockShort.h"
#include "magnetometer.h"
#include "ultrasonic.h"
#include "reflectance.h"

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
	
        InfraredSeeker::Initialize();
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

void loopy() {
	get_ball_angle();
	delay(500);
}

void loop() {
	calibration_mode_switch = digitalRead(CALIBRATION_MODE_SWITCH_PIN);
	if (!calibration_mode_switch) {
		magnetometer_calibrateMagnetometer(); 
	}
	if (!digitalRead(SAVE_HEADING_BUTTON_PIN)) { // allows the heading to be saved during use
		magnetometer_saveHeading();
	}
        readReflectance();
	//readIRsensors();
	//printIRsensors();
	//get_ball_angle();
	
	angle_to_goal = magnetometer_getAngleToTarget();
	
        //InfraredResult InfraredBall = InfraredSeeker::ReadAC();
        //ball_distance = 90.147-0.4345*InfraredBall.Strength;
<<<<<<< HEAD
	 //ball_distance = InfraredBall.Strength;
=======
	 ball_distance = InfraredBall.Strength;
   ball_angle = ir_sensor_angles[InfraredBall.Direction];
   ball_detected = 1;
>>>>>>> d40b7b986851da57064b3e57f9791394d5930ba7
         

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

float frequency(int note) {
	return 440 * pow(2.0, (note - 69) / 12.0);
}

void playNote(int pitch, int duration) {
	tone(BUZZER, (int)round(frequency(pitch)), duration);	
	delay(duration+5);
	noTone(BUZZER);
	delay(5);
}

void setNoteDurations(int bpm) {
	crotchet = 60000 / bpm;
}

void playTune(float tune[][2], int tempo, int arrayLength) {
	setNoteDurations(tempo);
	for(int x=0; x<arrayLength; x+=1) {
		if(tune[x][0] == 0) {
			delay(tune[x][1]*crotchet);
		} else {
			playNote(tune[x][0], tune[x][1]*crotchet);
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

void send_output() {
	Serial.print(ball_detected);Serial.print(" ");
	Serial.print(ball_angle);Serial.print(" ");
	Serial.print(ball_distance);Serial.print(" ");
	Serial.print(front_range);Serial.print(" ");
	Serial.print(back_range);Serial.print(" ");
	Serial.print(angle_to_goal);Serial.print(" ");
	Serial.print(calibration_mode_switch);Serial.print(" ");
	Serial.print(reflectance);Serial.print(" ");
	Serial.println(";");
}

int posneg(int input) {
  if(input > 180) {
    return(input - 360); 
  }
  return input;
}

float get_ball_angle() {
	DEBUG("ball angle called!");
	int small=2000;
    int sens=0;
    for(int x=0; x<NUM_SENSORS; x++) {
		
		//Serial.print(analogRead(analog_sensor_pins[x]));
		//Serial.print(" ");
		
		if(analogRead(analog_sensor_pins[x]) < small) {
			small=analogRead(analog_sensor_pins[x]);
			sens=x;
		}
     }

     if(small>900) {
		 ball_detected = 0;
                 ball_angle = 999;
	 } else {		 
                ball_detected = 1;
                 ball_angle = sens*45;
                 ball_angle = posneg(ball_angle);
                   //InfraredResult InfraredBall = InfraredSeeker::ReadAC();
                   //ball_angle = ir_sensor_angles[InfraredBall.Direction];
                 
	 }
    ball_distance=small;
     
     
    
    //Serial.print(ball_angle);
    //Serial.println("");
}

