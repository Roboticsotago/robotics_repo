// NOTE: DFRobot sensor board must be jumpered correctly for TTL operation and have been programmed (EEPROM via serial) to passive PWM mode!
// See https://www.dfrobot.com/wiki/index.php/URM37_V3.2_Ultrasonic_Sensor_(SKU:SEN0001)

const int trig = 5;
const int pwm = 8;
const int unit_division_factor = 40; // Supposedly 50 us/cm but Tobias found that 40 gave better accuracy.
int us_distance;
const int RANGE_SENSOR_TIMEOUT = 15000; // microseconds for pulseIn() timeout (to avoid excessive blocking delay)


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
