const int trig = 5;
const int pwm = 8;
const int unit_division_factor = 40;
int us_distance;

void ultrasonic_setup() {
	pinMode(trig, OUTPUT);
}

int getUSDistance() {
	digitalWrite(trig, LOW);
	digitalWrite(trig, HIGH);
	
	us_distance = pulseIn(pwm, LOW);
	return us_distance / unit_division_factor;
}
