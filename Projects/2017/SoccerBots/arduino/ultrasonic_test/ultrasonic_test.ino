const int trig = 5;
const int pwm = 8;
const int unit_division_factor = 40;
int us_distance;

void setup() {
	pinMode(trig, OUTPUT);
	Serial.begin(9600);
}

void loop() {
	digitalWrite(trig, LOW);
	digitalWrite(trig, HIGH);
	
	us_distance = pulseIn(pwm, LOW);
	Serial.println(us_distance / unit_division_factor);
	
	delay(20);
}
