const int STEP_PIN = 10;
//const int DIR_PIN = ??;

const int STEPS_PER_REV = 200;
const int degrees_per_step = 360 / STEPS_PER_REV;
const int LOW_TIME = 1; // ms
// If the high and low pulse durations are limited to 1 ms, that's a speed limit of 150 RPM:
// 1 / (2 ms) = 500 pps, 500 * 60 / 200 = 150


void setup() {
  pinMode(STEP_PIN, OUTPUT);
  Serial.begin(9600);
}


void spin(float rpm, int revs) {
//  float rps = rpm / 60;
//  float steps_per_sec = rps / degrees_per_step;
  float step_period = 1 / (rpm / 60.0 * STEPS_PER_REV) * 1000;  // ms
  int high_time = round(step_period) - LOW_TIME;
  Serial.println(high_time);

  for(int x = 50 + high_time; x >= high_time; x -= 5) {
    digitalWrite(STEP_PIN, HIGH);
    delay(x);
    digitalWrite(STEP_PIN, LOW);
    delay(LOW_TIME);
  }

  for (long int i = 0; i < revs; i++) {
  //  Serial.println(i);
    digitalWrite(STEP_PIN, HIGH);
    delay(high_time);
    //delay(1/steps_per_sec * 1000 - 1);
    digitalWrite(STEP_PIN, LOW);
    delay(LOW_TIME);
  }
  
  // TODO: ramp down after a certain number of iterations?
  for(int x = high_time; x <= 50 + high_time; x += 5) {
    digitalWrite(STEP_PIN, HIGH);
    delay(x);
    digitalWrite(STEP_PIN, LOW);
    delay(LOW_TIME);
  }
}


void loop() {
  spin(60, 600);
  delay(1000);
}
