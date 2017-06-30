const int trig = 5;
const int pwm = 8;
const int unit_division_factor = 40;
int us_distance;
const int RANGE_SENSOR_TIMEOUT = 15000; // us


void ultrasonic_setup() {
	pinMode(trig, OUTPUT); digitalWrite(trig, HIGH);
        pinMode(pwm, INPUT);
}

int getUSDistance() {
	digitalWrite(trig, LOW);
        //delay(50);    
	digitalWrite(trig, HIGH);
	
	us_distance = pulseIn(pwm, LOW, RANGE_SENSOR_TIMEOUT);
	return us_distance / unit_division_factor;
}
