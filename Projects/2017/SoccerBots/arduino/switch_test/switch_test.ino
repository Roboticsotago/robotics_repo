const int switch_pin = 18;

void setup(){
  pinMode(switch_pin, INPUT); digitalWrite(switch_pin,HIGH);
  Serial.begin(9600);
}

void loop(){
  Serial.println(digitalRead(switch_pin));
  delay(50);
}
