#include <math.h>

const float TAU = asin(1);

// Oh, there's already a degrees() in math.h?
//float degrees(float radians) {return 360.0 / TAU * radians;}

void atan2_test() {
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			Serial.print("atan2("); Serial.print(x); Serial.print(","); Serial.print(y); Serial.print(") = ");
			Serial.println(degrees(atan2(x,y)));
		}
	}
}

void setup() {
	Serial.begin(9600);
}

void halt() {
	while (1) {}
}

void loop() {
	atan2_test();
	halt();
}
