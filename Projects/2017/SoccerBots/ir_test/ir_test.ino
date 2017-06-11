#include <math.h>
const int IR_1 = 2;
const int IR_2 = 3;
const int IR_3 = 4;
const int IR_4 = 5;
const int NUM_SENSORS = 8;
const int sensor_pins[] = {2,3,4,5,6,7,8,9};

float IR_COORDINATES[NUM_SENSORS][2] = {{0.0,1.0},{0.71,0.71},{1.0,0.0},{0.71,-0.71},{0.0,-1.0},{-0.71, -0.71},{-1.0, 0.0},{-0.71, 0.71}};

int ir_val;
const float TAU = 2 * PI;

void setup() {

	for(int n=0; n<NUM_SENSORS; n++) {
		pinMode(sensor_pins[n], INPUT);
	}
	Serial.begin(9600);
	Serial.println("Welcome Uno");
}

void loop() {
  /*ir_val = digitalRead(IR_1);
  
  if(ir_val == 0) {
    Serial.print("1");
  } else {
    Serial.print("0");
  }
  
  delay(20);*/
  
  Serial.println(ball_angle());
  delay(5000);
 // rad2degTest();
 // while (1) {}
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

float ball_angle() {
 Serial.println("ball angle called!");
  int x_total; int y_total; int count; 
  for(int n=0; n<NUM_SENSORS; n++) {
    if (!digitalRead(sensor_pins[n])) {
      x_total += IR_COORDINATES[n][0];
      y_total += IR_COORDINATES[n][1];
      Serial.print(n);
      Serial.print(" (");
      Serial.print(IR_COORDINATES[n][0]);
      Serial.print(",");
      Serial.print(IR_COORDINATES[n][1]);
      Serial.println(")");
      count += 1;
    }
  }
	float x_average = x_total/count;
	float y_average = y_total/count;
	return normaliseDegrees(rad2deg(atan(y_average/x_average)));
	if(y_average > 0){
		return atan2(x_average,y_average);
	}
	else if(y_average < 0){
		if (x_average > 0){
			return PI+atan2(x_average,y_average);
		}else{
			return -PI+atan2(x_average,y_average);
		}
	}else if(y_average == 0){
		if (x_average > 0){
			return PI/2;
		}else{
			return-PI/2;
		}
	}
  } 

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

