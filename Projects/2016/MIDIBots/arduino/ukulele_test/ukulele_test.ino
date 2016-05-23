// Testing multiple servo motors

#include <Servo.h> 
 
Servo servo1;

int pos = 0;    // variable to store the servo position 

const int STRUM_MAX_POS = 100; //sets MAX and MIN servo postions for all strings
const int STRUM_MIN_POS = 80;

const int G_MAX_POS = 85;
const int G_MIN_POS = 82;

const int C_MAX_POS = 87; 
const int C_MIN_POS = 85; 

const int E_MAX_POS = 91; 
const int E_MIN_POS = 88; 

const int A_MAX_POS = 94;
const int A_MIN_POS = 92;

const int SERVO_TIME = 200           ; // ms delay to allow motion to happen

const int SERVO_1_PIN = 11; //Servo is connected to pin 11(pwm)

byte byteRead; //temporary storage of serial bytes

void setup() 
{ 
	pinMode(SERVO_1_PIN, OUTPUT);
	digitalWrite(SERVO_1_PIN, LOW);
	servo1.attach(SERVO_1_PIN);	
        Serial.begin(9600); //start serial protocol
} 
 
void G() //A function to play the G string on the Uke
{
	servo1.write(G_MAX_POS);
	delay(SERVO_TIME);
	servo1.write(G_MIN_POS);
	delay(SERVO_TIME);
}
void C()
{
	servo1.write(C_MAX_POS);
	delay(SERVO_TIME);
	servo1.write(C_MIN_POS);
	delay(SERVO_TIME);
}

void E()
{
	servo1.write(E_MAX_POS);
	delay(SERVO_TIME);
	servo1.write(E_MIN_POS);
	delay(SERVO_TIME);
}

void A()
{
	servo1.write(A_MAX_POS);
	delay(SERVO_TIME);
	servo1.write(A_MIN_POS);
	delay(SERVO_TIME);
}

void strum()
{
	servo1.write(STRUM_MAX_POS);
	delay(SERVO_TIME);
	servo1.write(STRUM_MIN_POS);
	delay(SERVO_TIME);
}

void manual(int max_pos, int min_pos, int servo_delay)
{
  servo1.write(max_pos);
  delay(servo_delay);
  servo1.write(min_pos);
  delay(servo_delay);
}
void serial_read()
{
  if (Serial.available() > 0) //checks if a serial device is listening
  {
    byteRead = Serial.read(); //captures serial data byte
    if(byteRead == 'G') //checks weather the user has typped "G"
    {
	G(); //if so call the G function which will play the G string on the ukulele.
    Serial.println("recived G"); //Alerts the user that it has been played
	
  }
  if(byteRead == 'C')
    {
	C();
 Serial.println("recived C");
  }
  if(byteRead == 'E')
    {
	E();
 Serial.println("recived E");
  }
  if(byteRead == 'A')
    {
	A();
	 Serial.println("recived A");
  }
  if(byteRead == 'S')
    {
	strum();
 Serial.println("recived strum");
  }
}
}

void loop() {
	serial_read();
}


