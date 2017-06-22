/*
  Magnetometer Test
  Reads data from the LIS3MDL sensor and calculate a compass heading and can
  return the angle to a specified heading
  The y-axis points in the forwards direction of the robot

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
float reference_heading;

LIS3MDL mag;
LIS3MDL::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

// calibrate the magnetometer, the magnetometer must be moved through its full axis of rotation while calibrating
void calibrateSensor(){
  for (int i = 0; i < 200; i += 1) {
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
    snprintf(report, sizeof(report), "min: {%+6d, %+6d, %+6d}   max: {%+6d, %+6d, %+6d}",
      running_min.x, running_min.y, running_min.z,
      running_max.x, running_max.y, running_max.z);
    Serial.println(report);

    delay(50);
  }

  // calculate calibrated origin
  cal_x = (running_max.x + running_min.x) / 2;
  cal_y = (running_max.y + running_min.y) / 2;
}

// read raw data from magnetometer and adjust from calibration
void read() {
  mag.read();

  // adjust raw data for calibration
  mag.m.x -= cal_x;
  mag.m.y -= cal_y;

  // print adjusted values
  snprintf(report, sizeof(report), "M: %6d, %6d, %6d",
    mag.m.x, mag.m.y, mag.m.z);
  Serial.println(report);
}

// returns the compass heading from magnetometer, range [0, 360]
float getCompassHeading() {
    read();
    float heading = calcAngle(mag.m.x, mag.m.y) * -1; // compass heading is angle clockwise from North, hence the * -1
    if (heading < 0) {
        return heading + 360.0;
    } else {
        return heading;
    }
}

// calculate the angle of point (x, y)
float calcAngle(int x, int y) {
  return degrees(atan2(x, y));
}

// stores the compass angle that is the desired direction
float saveHeading() {
  reference_heading = getCompassHeading();
}

// returns the angle that the robot is off the reference_heading, range [-180, 180]
float getRelativeAngle(float my_heading) {
  float angle_diff = (my_heading - reference_heading) * -1.0;

  Serial.print("Actual heading: "); Serial.print(my_heading);
  Serial.print(" Angle difference: "); Serial.println(angle_diff);

  if (angle_diff < -180.0) {
    return angle_diff + 360.0;
  } else if (angle_diff > 180.0) {
    return angle_diff - 360.0;
  } else {
    return angle_diff;
  }
}

// checks to see if facing target dirrection
int isTargetGoal(float my_heading) {
  if ((my_heading < 90.0) && (my_heading > -90.0)) {
    return 1;
  } else {
    return 0;
  }
}

// functions to run at start for testing
void startFunctionsTest() {
  // calibrate the sensor
  Serial.println("Calibration started");
  calibrateSensor();
  Serial.println("Calibration finished");

  // store the current direction after short delay
  Serial.print("Saving direction in: ");
  for (int n = 4; n > 0; n += 1) {
    Serial.print(n);
    delay(1000);
  }
  Serial.println();
  saveHeading();
  Serial.print("Saved heading is: "); Serial.println(reference_heading);
  delay(2000);
}

void loopTest() {
  Serial.print("Angle to desired heading is: ");
  Serial.println(isTargetGoal(getRelativeAngle(getCompassHeading())));
}

void setup() {
  Serial.begin(9600);
  Serial.println("Magnetometer Test");
  Wire.begin();

  if (!mag.init()) {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1);
  }
  mag.enableDefault();

  startFunctionsTest();
}

void loop() {
  loopTest();
  delay(1000);
}
