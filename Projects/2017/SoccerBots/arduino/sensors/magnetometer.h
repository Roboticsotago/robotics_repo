/*
  Magnetometer
  Reads data from the LIS3MDL sensor and calculate a compass heading, aswell as
  return the angle to a specified heading
  The y-axis points in the forwards direction of the robot for a correct heading

TODO:
- implement correction for magmetic north
*/

/*
The sensor outputs provided by the library are the raw 16-bit values
obtained by concatenating the 8-bit high and low magnetometer data registers.
They can be converted to units of gauss using the
conversion factors specified in the datasheet for your particular
device and full scale setting (gain).

Example: An LIS3MDL gives a magnetometer X axis reading of 1292 with its
default full scale setting of +/- 4 gauss. The GN specification
in the LIS3MDL datasheet (page 8) states a conversion factor of 6842
LSB/gauss (where LSB means least significant bit) at this FS setting, so the raw
reading of 1292 corresponds to 1292 / 6842 = 0.1888 gauss.
*/

#include <Wire.h>
#include <LIS3MDL.h>
#include <math.h>

char report[80];
int cal_x, cal_y;
float target_heading;
float getCompassHeading();

LIS3MDL mag;
LIS3MDL::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

float saveHeading() { // stores the compass angle that is the desired direction
	target_heading = getCompassHeading();
}

void magnetometer_reset() { // reset the running max and min values for each axis to allow a complete calibration
	running_min.x =  32767;
	running_min.y =  32767;
	running_min.z =  32767;
	running_max.x = -32768;
	running_max.y = -32768;
	running_max.z = -32768;
}

void calibrateSensor() { // calibrate the magnetometer, the magnetometer must be moved through its full axis of rotation while calibrating
	DEBUG("starting magnetometer calibration");
	magnetometer_reset();
	while(!digitalRead(CALIBRATION_MODE_SWITCH_PIN)){
		mag.read();

		// minimum values
		running_min.x = min(running_min.x, mag.m.x);
		running_min.y = min(running_min.y, mag.m.y);
		running_min.z = min(running_min.z, mag.m.z);

		// maximum values
		running_max.x = max(running_max.x, mag.m.x);
		running_max.y = max(running_max.y, mag.m.y);
		running_max.z = max(running_max.z, mag.m.z);

		// print limits
		// snprintf(report, sizeof(report), "min: {%+6d, %+6d, %+6d}   max: {%+6d, %+6d, %+6d}",
		//   running_min.x, running_min.y, running_min.z,
		//   running_max.x, running_max.y, running_max.z);
		// DEBUG(report);
	}

	// calculate new origin
	cal_x = (running_max.x + running_min.x) / 2; // if latency rethink placement of these
	cal_y = (running_max.y + running_min.y) / 2;
	saveHeading();
	DEBUG("ending magnetometer calibration");
}

void read() { // read raw data from magnetometer and adjust from calibration
	mag.read();

	// adjust raw data for calibration
	mag.m.x -= cal_x;
	mag.m.y -= cal_y;

	// print adjusted values
	// snprintf(report, sizeof(report), "M: %6d, %6d, %6d", mag.m.x, mag.m.y, mag.m.z);
	// DEBUG(report);
}

float calcAngle(int x, int y) { // calculate the angle of point (x, y)
	return degrees(atan2(x, y));
}

float getCompassHeading() { // returns the compass heading from magnetometer, range [0, 360]
	read();
	float heading = calcAngle(mag.m.x, mag.m.y) * -1; // compass heading is angle clockwise from North, hence the * -1
	if (heading < 0) {
		return heading + 360.0;
	} else {
		return heading;
	}
}

float getRelativeAngle(float actual_heading) { // returns the angle that the robot is off the target heading, range [-180, 180]
	float angle_diff = (actual_heading - target_heading) * -1.0;

	DEBUG_NOEOL("Actual heading: "); DEBUG_NOEOL(actual_heading);
	DEBUG_NOEOL(" Angle difference: "); DEBUG(angle_diff);

	if (angle_diff < -180.0) {
		return angle_diff + 360.0;
	} else if (angle_diff > 180.0) {
		return angle_diff - 360.0;
	} else {
		return angle_diff;
	}
}

int isTargetdirrection(float actual_heading) { // checks to see if facing in the general target dirrection
	if ((actual_heading < 70.0) && (actual_heading > -70.0)) {
		return 1;
	} else {
		return 0;
	}
}

float getAngleToTarget() { // returns the angle needed to turn towards the target heading
	return getRelativeAngle(getCompassHeading());
}

void magnetometerTestLoop() {
	DEBUG_NOEOL("Angle to desired heading is: ");
	DEBUG(isTargetdirrection(getAngleToTarget()));
}

void magnetometerSetup() {
	// Serial.begin(9600);
	// DEBUG("Magnetometer Test");
	Wire.begin();
	if (!mag.init()) {
		DEBUG("Failed to detect and initialize magnetometer!");
		while (1);
	}
	mag.enableDefault();
}
