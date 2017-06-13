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
const int IR_THRESHOLD = 870; 

int ir_val;
const float TAU = 2 * PI;

void setup() {

	for(int n=0; n<NUM_SENSORS; n++) {
		//pinMode(sensor_pins[n], INPUT);
                pinMode(analog_sensor_pins[n], INPUT);

	}
	Serial.begin(9600);
	Serial.println("Welcome Uno");
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
	printIRsensors();
	delay(100);
}

float readIRsensor(int sensor_num){
	int reading = analogRead(analog_sensor_pins[sensor_num]);
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

//~ float ball_angle() {
 //~ Serial.println("ball angle called!");
  //~ int x_total; int y_total; int count; 
  //~ for(int n=0; n<NUM_SENSORS; n++) {
	 //~ 
	  //~ x_total += IR_COORDINATES[n][0];
	  //~ y_total += IR_COORDINATES[n][1];
	  //~ Serial.print(n);
	  //~ Serial.print(" (");
	  //~ Serial.print(IR_COORDINATES[n][0]);
	  //~ Serial.print(",");
	  //~ Serial.print(IR_COORDINATES[n][1]);
	  //~ Serial.println(")");
	  //~ count += 1;
    //~ }
  //~ }
	//~ float x_average = x_total/count;
	//~ float y_average = y_total/count;
	//~ return normaliseDegrees(rad2deg(atan(y_average/x_average)));
	//~ if(y_average > 0){
		//~ return atan2(x_average,y_average);
	//~ }
	//~ else if(y_average < 0){
		//~ if (x_average > 0){
			//~ return PI+atan2(x_average,y_average);
		//~ }else{
			//~ return -PI+atan2(x_average,y_average);
		//~ }
	//~ }else if(y_average == 0){
		//~ if (x_average > 0){
			//~ return PI/2;
		//~ }else{
			//~ return-PI/2;
		//~ }
	//~ }
  //~ } 

  //~ if(x_average==0 &&  y_average>0) {
    //~ return 0;
  //~ } else if(x_average==0 && y_average<0) {
    //~ return 180;
  //~ } else if(x_average>0 && y_average==0) {
    //~ return 90;
  //~ } else if(x_average<0 && y_average==0) {
    //~ return -90;
  //~ } else if(y_average > 0) {
    //~ 
  //~ } else if(y_average < 0) {
    //~ return 180 + atan(y_average/x_average) * (180/PI);
  //~ }   

