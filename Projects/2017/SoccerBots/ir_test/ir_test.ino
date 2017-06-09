const int IR_1 = 2;
const int IR_2 = 3;
const int IR_3 = 4;
const int IR_4 = 5;

int IR_COORDINATES[2][2] = {{0,1},{1,1}};

int ir_val;

void setup() {
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);  
  Serial.begin(9600);
}

void loop() {
  /*ir_val = digitalRead(IR_1);
  
  if(ir_val == 0) {
    Serial.print("1");
  } else {
    Serial.print("0");
  }
  
  delay(20);*/
  
  ball_angle({IR_1,IR_2});
  
}

float ball_angle(int sensors[]) {
  int x_total; int y_total; int count;
  for(n=0; n<sizeof(sensors); n++) {
    if !digitalRead(sensors[n]) {
      x_total += IR_COORDINATES[n][0];
      y_total += IR_COORDINATES[n][1];
      count += 1;
    }
  }
  float x_average = x_total/count;
  float y_average = y_total/count;
  if(x==0 && y>0) {
    return 0;
  } else if(x_average==0 && y_average<0) {
    return 180;
  } else if(x_average>0 && y_average==0) {
    return 90;
  } else if(x_average<0 && y_average==0) {
    return -90;
  } else if(y_average > 0) {
    return atan(y_average/x_average) * (180/PI);
  } else if(y_average < 0) {
    return 180 + atan(y_average/x_average) * (180/PI);
  }   
    
}
  
