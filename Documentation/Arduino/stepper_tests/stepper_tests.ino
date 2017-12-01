// Stepper test, CME 2015-11-03

const int STEP_PIN = 2;
const int DIRECTION_PIN = 3;
const int SINE_STEPS = 200;
const int MIN_PERIOD = 480;
int Direction = 0;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
}

void loop() {
  for (int i = 0; i < SINE_STEPS; i++) {
      digitalWrite(STEP_PIN, HIGH); delayMicroseconds((SINE_STEPS-i) + MIN_PERIOD);
      digitalWrite(STEP_PIN, LOW ); delayMicroseconds((SINE_STEPS-i) + MIN_PERIOD);
//      digitalWrite(STEP_PIN, HIGH); delayMicroseconds(round(cos((float)i / (float)SINE_STEPS)) * 2000 + 1000);
//      digitalWrite(STEP_PIN, LOW ); delayMicroseconds(round(cos((float)i / (float)SINE_STEPS)) * 2000 + 1000);
  }
  // Ramp speed down:
  for (int i = 0; i < SINE_STEPS; i++) {
      digitalWrite(STEP_PIN, HIGH); delayMicroseconds(i + MIN_PERIOD);
      digitalWrite(STEP_PIN, LOW ); delayMicroseconds(i + MIN_PERIOD);
//      digitalWrite(STEP_PIN, HIGH); delayMicroseconds(round(cos((float)i / (float)SINE_STEPS)) * 2000 + 1000);
//      digitalWrite(STEP_PIN, LOW ); delayMicroseconds(round(cos((float)i / (float)SINE_STEPS)) * 2000 + 1000);
  }
}

void loop_() {
  for (int i = 0; i < 100; i++) {
    digitalWrite(STEP_PIN, HIGH); delayMicroseconds(1000);
    digitalWrite(STEP_PIN, LOW); delayMicroseconds(1000);
  }
  Direction = !Direction;
  digitalWrite(DIRECTION_PIN, Direction);
  delayMicroseconds(10000);
}

