void setup() {
  pinMode(10, OUTPUT);
}

const int steps_per_rev = 200;
const int degrees_per_step = 360 / steps_per_rev;


void spin(float rpm) {
  for(int x = 1000; x >= ; x -= 50) {
    digitalWrite(10, HIGH);
    delay(x);
    digitalWrite(10, LOW);
    delay(1);
  }
  float rps = rpm / 60;
  float steps_per_sec = rps / degrees_per_step;
  digitalWrite(10, HIGH);
  delay(1/steps_per_sec * 1000 - 1);
  digitalWrite(10, LOW);
  delay(1);
}


void loop() {
  spin(60);
}
