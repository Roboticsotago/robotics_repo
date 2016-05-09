// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.


#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
const int max_pos = 100;
const int min_pos = 0;

const int servo_pin = 11;
 
void setup() 
{ 
  pinMode(servo_pin, OUTPUT);
  myservo.attach(servo_pin);  // attaches the servo on pin 9 to the servo object 
} 
 
void playbeat (int posttime)
{
  myservo.write(max_pos);
  delay(80);
  myservo.write(min_pos);
 delay(posttime-80);
}
 
void loop() 
{ 
  playbeat(400);
  playbeat(400);
  playbeat(200);
  playbeat(200);
  playbeat(200);
  playbeat(200);
} 
