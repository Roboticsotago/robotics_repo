/*
  Magnetometer Test
  Reads data from the LIS3MDL sensor and calculate a compass heading

TODO:
- troubleshoot compass heading when y < 0
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

LIS3MDL mag;
LIS3MDL::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

char report[80];
int cal_x, cal_y;

// calibrate the sensor, the sensor must be moved through its full axis while it is calibrating
void calibrateSensor(){
  int i = 0;
  while (i < 200) {
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
    i += 1;
  }

  // calculate calibrated origin
  cal_x = (running_max.x + running_min.x) / 2;
  cal_y = (running_max.y + running_min.y) / 2;
}

// read raw data from the sensor and adjust from calibration
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

// compute compass heading
float computeHeading(int mX, int mY) {
  float heading;

  // calculate heading using arc tangent
  if (mY > 0) {
    heading = 90 - (atan2(mX, mY) * (180 / PI));
  } else if (mY < 0) { // currently behaves undesirability
    float val = mX / mY;
    heading = 270 - (atan(val) * (180 / PI));
  } else if (mY == 0 && mX < 0) {
    heading = 180.0;
  } else if (mY == 0 && mX > 0) {
    heading = 0.0;
  }

  // return compass heading, range [0, 360]
  return heading;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!mag.init()) {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1);
  }
  mag.enableDefault();

  // calibrate the sensor before beginning
  calibrateSensor();
}

void loop() {
  read();

  // print compass heading
  Serial.println(computeHeading(mag.m.x, mag.m.y));

  delay(100);
}

