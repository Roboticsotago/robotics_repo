const int steps_per_rev = 200;
const int degrees_per_step = 360 / steps_per_rev;
const int motor_pin = 10;

void spin(float rpm, float num_rotations) {
  min_delay = 300/rpm - 1
  for(int i = 0; i <= num_rotations*200; i++) {
    digitalWrite(motor_pin, HIGH);
    delay(min_delay);
    digitalWrite(motor_pin, LOW);
    delay(1);
  }
}

void accel(float rpm) {
  for(float current_rpm = 0; current_rpm <= rpm; current_rpm += rpm / 10) {
    spin(current_rpm, 0.1);
  } 
}

void decel(float initial_rpm) {
  for(float current_rpm = initial_rpm; current_rpm >= 0; current_rpm -= initial_rpm / 10) {
    spin(current_rpm, 0.1);
  }
  digitalWrite(motor_pin, HIGH)
}

void setup() {
  pinMode(motor_pin, OUTPUT);
}

void loop() {
  accel(60);
  spin(60, 10);
  decel(60);
  delay(5000);
}
