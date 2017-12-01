// Test program for Pololu-style Allegro stepper driver carrier, CME 2015-11-02
// Wantai 42BYGHW811 test: at 4 kHz (1200 RPM) a lower frequency vibration starts to appear.
// Interestingly, it won't start reliably at high frequencies - you have to ramp it up I guess.  Acceleration setting in CNC control!  Reaches 7 kHz (2100 RPM) OK.  Fails to reach 10 kHz (3000 RPM) with ~1 A current limit.  8 kHz, indeed.

// It's also very rough and skips steps at low speeds as well.  The severity of this seems to be somewhat sensitive to the current limiting, with more current being a bigger problem.

// 17HS42 motor: 8 kHz is very marginal. More current not necc better!
// Wantai can achieve 10 kHz but nees more current to do so.

// Microstepping: initial experiment with 4x microstepping: I think higher speeds MIGHT be achievable, and the resonance problem is reduced.  Of course, you need 4x the pulse frequency to achieve the same speeds!  The Wantai can do 90 kHz -> 3375 RPM.  120 kHz -> 4500 RPM (erm, sometimes! It's pretty marginal).

// 2015-11-04: Have rigged up the 24-V power supply and the modded-for-24-V RAMPS board for testing.  Need to pull the enable pin for the stepper drive low.  Also, the RAMPS is jumpered for 16x microstepping, so the frequency-to-RPM mapping is changed again.  192 kHz would be 3600 RPM, I think
// 192 kHz / 200 / 16 -> min^-1 = 3600
// With delayMicroseconds(), I think it's a bit tough to reach high frequencies, and certainly not with any precision.  I managed to get it to just over 30 kHz
//17HS19 reached 690RPM with no load 

// For initial testing:
//const int STEP_PIN = 2;
//const int DIR_PIN = 3;

// RAMPS E0 stepper:
const int STEP_PIN = 26;
const int DIR_PIN = 28;
const int EN_PIN = 24; // Maybe this has to be set high^H^H^H^Hlow(!) for anything much to happen...

const int STEPS_PER_REVOLUTION = 200; // Depends on the stepper motor
const int MICROSTEPPING = 1; // Typically 16 on RAMPS boards for 3D printing.

// Should define constants for the timing signals too..


const int PULSE_LOW_TIME = 1000; // A4988
const int PULSE_HIGH_TIME = 1000;
// MadRobot router's YAKO drives have 2500 us

const int LED_PIN = 13;

const int MIN_FREQ = 50;
const long int MAX_FREQ = 1500; // Max step frequency in Hz.  Typical stepper motors are 1.8 degrees/step (200 steps/revolution), although microstepping could increase this up to 16-fold.  Hmm, but I'm seeing the frequency not go any higher than about 12 kHz on the scope..may need more optimised code...
const float FREQ_STEP = 10;

float freq = 0.0;
long int period_int = 0; // Global variable for current stepping pulse period (so it can be reused without recomputing every time).

int current_direction = 1;

void setup() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN,  OUTPUT);
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
  cw();
  pinMode(LED_PIN, OUTPUT);
}

// Compute and store the per-symbol period as an integer:
void set_period(float period) {
  period_int = (long int)round(period / 2.0); // Account for time for both high and low period.
}

void step() {
  digitalWrite(STEP_PIN, HIGH); digitalWrite(LED_PIN, HIGH); delayMicroseconds(period_int);
  digitalWrite(STEP_PIN, LOW); digitalWrite(LED_PIN, LOW); delayMicroseconds(period_int);
}

// Set direction to clockwise:
void cw() {
  digitalWrite(DIR_PIN, HIGH);
  // TODO: direction hold delay
}

// ...and counterclockwise:
void ccw() {
  digitalWrite(DIR_PIN, LOW);
  // TODO: direction hold delay
}

void _loop() {
}


void loop() {
  // Ramp speed up:
  // TODO: try exponential acceleration curve
  current_direction = !current_direction;
  if(current_direction){
    cw();
  }
  else{
    ccw();
  }
  
  for (freq = MIN_FREQ; freq < MAX_FREQ; freq = freq + FREQ_STEP) {
    set_period(1.0 / freq * 1e6); // Convert to period in microseconds
    step(); step(); step(); step(); step(); step(); step(); step();
  }

  // I've noticed the speed increase during this loop. Ah, because it's not having to do so much computation!  Let's compute and store period in a global variable to speed it up even more.
  // Run for a while at max...
  // TODO: have num iterations here based on time rather than a fixed number!
  for (int i = 0; i < 2000; i++) {
     step(); step(); step(); step(); step(); step(); step(); step();
  }
  
  // ...and ramp it down again:
  for (freq = MAX_FREQ; freq > MIN_FREQ; freq = freq - FREQ_STEP) {
    set_period(1.0 / freq * 1e6); // Convert to period in microseconds
    step(); step(); step(); step(); step(); step(); step(); step();
  }
}

// What about variable speed 
