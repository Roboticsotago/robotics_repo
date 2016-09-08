// Fan control firmware for 2016 MIDIBots
// Possibly doesn't need to use MIDIBot library, but the MIDI control is handy for testing.

/*
TODO:
[ ] Add code to read from pressure sensor. Should take initial reading for "zero" reference at startup, since the sensor is absolute, not relative (differential) and ambient pressure can vary.
[ ] Add code to control fan speed to maintain desired pressure. PID control? Have some sanity checks, e.g. spin down if fault detected/suspected (if monitoring the fan's tachometer output).
https://www.youtube.com/watch?v=UR0hOmjaHp0
http://robotics.stackexchange.com/questions/10127/pid-with-position-and-velocity-goal/10128#10128
http://robotics.stackexchange.com/questions/5260/the-aerial-refueling-problem-sketch-of-a-feedback-controller
https://www.youtube.com/watch?v=H4YlL3rZaNw
https://www.youtube.com/watch?v=taSlxgvvrBM
*/

#include <Servo.h> 
#include <Wire.h>
#include <MIDIBot.h>
MIDIBot fanBot;

Servo partyPopperServo;

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

//#define DEBUG 1
#define NO_PID 1
	
const unsigned char OSS = 0;  // Oversampling Setting

long target_pressure = 250; // Pa relative to initial reference reading

#ifdef DEBUG 
int control_enabled = 1;
#else
int control_enabled = 0;
#endif

unsigned long last_micros = 0;

const float Kp = 0.001,
	Ki = 0.0003 * 300000, //original constant were for 300ms sampling period //0.0003
	Kd = 0.0001 * 300000; //0.0001

long ref_pressure = 0;
float integral = 0;
long prev_error = 0;
float control = 0;

const int PARTY_POPPER_MAX = 170; 
const int PARTY_POPPER_MIN = 40;

// Calibration values
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;


// MIDI note mapping:
const int FAN_NOTE_ON = 1;
const int FAN_NOTE_OFF = 0;
const int FAN_UP_NOTE = 5;
const int FAN_DOWN_NOTE = 6;

const int PARTY_POPPER_FIRE_NOTE =  3;
const int PART_POPPER_RELEASE_NOTE= 4;


// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5; 

short temperature;
long pressure;

// Use these for altitude conversions
const float p0 = 101325;     // Pressure at sea level (Pa)
float altitude;

const int PWM_FREQUENCY = 25000; // Intel 4-wire fan spec. The Delta fan datasheet indicates 30 kHz .. 300 kHz, however.

void setup() {
#ifdef DEBUG 
	Serial.begin(115200);
#else
	fanBot.begin();
#endif

//	fan_speed(0);	// Bit dodgy setting this before the setup routine, but the fan spools up for some seconds otherwise.
	Wire.begin();
	bmp085Calibration();
	partyPopperServo.attach(SERVO_3_PIN);
	partyPopperServo.write(PARTY_POPPER_MIN);
#ifdef NOPID
	// Start up immediately
#else
	// NOTE: the Mega tends to output HIGH on pin 12 during programming, so we need to wait a bit for the fan to stop completely and the pressure to return to ambient before taking our initial reading.
	digitalWrite(LED_PIN, HIGH); // Light the LED to indicate we're busy
	delay(13000);
	digitalWrite(LED_PIN, LOW);
#endif

	calibrate();
	// Gah, can't get fan speed control to work when using MIDI input. Works OK using plain serial?! Dunno....
//	set_fan_speed(0.5);
	party_popper_fire();
}

int control_byte = 0;

// For testing:
int fan_speed = 0;
//const int FAN_SPEED_INCREMENT = 5,
const int  MIN_SPEED = 0,
	MAX_SPEED = 255;
/*
void speed_increase() {
	fan_speed += FAN_SPEED_INCREMENT ;
	if (fan_speed > MAX_SPEED) {fan_speed = MAX_SPEED;}
	set_fan_speed(fan_speed / 255.0);
}

void speed_decrease() {
	fan_speed -= FAN_SPEED_INCREMENT;
	if (fan_speed < MIN_SPEED) {fan_speed = MIN_SPEED;}
	set_fan_speed(fan_speed / 255.0);
}
*/

void calibrate(){
	temperature = bmp085GetTemperature(bmp085ReadUT());
	ref_pressure = bmp085GetPressure(bmp085ReadUP());
//	Serial.println(ref_pressure);
}

int rel_pressure(){
	temperature = bmp085GetTemperature(bmp085ReadUT());
	return bmp085GetPressure(bmp085ReadUP()) - ref_pressure;
} 

unsigned long elapsed_micros(){
	unsigned long current_micros =  micros();
	unsigned long micros_difference = current_micros - last_micros;
	last_micros = current_micros;
	return micros_difference;
}

void loop() {
#ifdef NOPID
	// Fan speed controlled directly by MIDI note-on velocity
#else
	unsigned long delta_t = elapsed_micros();
	
	long pressure = rel_pressure();

	//altitude = (float)44330 * (1 - pow(((float) pressure/p0), 0.190295));
	//fanBot.test_blink();

	// Proportional:
	long error = target_pressure - pressure;
	
	// Integral:
	if (control <= 1 && control >= 0)
	{
		integral += error / float(delta_t) ;
	}
	
	// Derivative:
	float derivative = (error - prev_error ) / float(delta_t);
	
	// Combined control:
	if(control_enabled){ 
		
		control = error * Kp + integral * Ki + derivative * Kd;
		set_fan_speed(control); //control
	} else{
		set_fan_speed(0);
	}
	
	
	prev_error = error;
#endif

#ifdef DEBUG 
	if (Serial.available()) {
		int control_byte = Serial.read();
		// TODO: error-checking?
		switch (control_byte) {
			case '+': target_pressure_increase(); break;
			case '=': target_pressure_increase(); break;
			case '-': target_pressure_decrease(); break;
			case '_': target_pressure_decrease(); break;
		}
	}
	Serial.print("Target pressure: ");
	Serial.println(target_pressure);
	Serial.print("pressure: ");
	Serial.println(pressure);
	Serial.print("Proportional: ");
	Serial.println(error * Kp, 6);
	Serial.print("Integral: ");
	Serial.println(integral * Ki, 6);
	Serial.print("Derivative: ");
	Serial.println(derivative * Kd, 6);
	Serial.print("Combined control: ");
	Serial.println(control, 6);
	Serial.println("Elapsed Micros:");
	Serial.println(elapsed_micros());
	Serial.println();
	delay(300);
#else
	fanBot.process_MIDI();
#endif
}


// Tricky low-level code from Chris for programming the PWM output for precise frequencies...
// Delta fan speed control requires a PWM signal with a base frequency of about 25 kHz.
// The standard Arduino PWM runs much lower than this, so we have to program it directly using the AVR control registers.

// On the Mega, we have timer1 attached to pins D11 and D12, D12 being the primary one.
// On "ordinary" Arduinos, it's on pins 9 and 10.  On the MIDIBot shield, pin 10 is Servo 2 (middle pin).
// Ideally we'd use the PWM MOSFET output on D6, but that uses Timer 0 and doesn't support frequency-accurate PWM, as far as I can tell.
// This will need an external circuit anyway to limit the current and block DC, so it's not a huge hassle to add a MOSFET circuit (with resistors and protection diode) to this as well.  It can be powered from one of the aux 12 V headers.
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
const int OUTPUT_PIN     = 12;
const int OUTPUT_PIN_AUX = 11;
#else
const int OUTPUT_PIN     = 10;
const int OUTPUT_PIN_AUX = 9;
#endif


// General settings:
const int prescale = 8;

// Some PWM control definitions:
#define TIMER_CLK_STOP		0x00	///< Timer Stopped
#define TIMER_CLK_DIV1		0x01	///< Timer clocked at F_CPU
#define TIMER_CLK_DIV8		0x02	///< Timer clocked at F_CPU/8
#define TIMER_CLK_DIV64		0x03	///< Timer clocked at F_CPU/64
#define TIMER_CLK_DIV256	0x04	///< Timer clocked at F_CPU/256
#define TIMER_CLK_DIV1024	0x05	///< Timer clocked at F_CPU/1024

// This defines which bits within the TCCRnB register determine the prescale factor:
#define TIMER_PRESCALE_MASK	0xF8        // 0B11111000

// Map desired prescale divider to register bits:
byte timer_prescale_bits(int prescale) {
	if (prescale == 1)
		return 0x01;
	else if (prescale == 8)
		return 0x02;
	else if (prescale == 64)
		return 0x03;
	else if (prescale == 256)
		return 0x04;
	else if (prescale == 1024)
		return 0x05;
	else
		return 0x00;	// Error?!
}


// Function for computing the PWM wrap limit ("top" value) based on the desired frequency (and the CPU clock speed and prescaler setting):
// TO0xBCDO: parameterise for prescale, rather than relying on it being globally defined?
unsigned int top(float frequency) {
	return round(F_CPU / (prescale * frequency) - 1);
}


void pwm(float frequency, float duty_cycle) {
	TCNT1 = 0;        // Reset timer counter
//	pwm_off(); // Maybe necessary to avoid stuck at 5 V condition? Nope, not enough...

	if (duty_cycle < 0.0) {duty_cycle = 0.0;}
	if (duty_cycle > 1.0) {duty_cycle = 1.0;}
	
	unsigned int wrap_limit = top(frequency);
	OCR1A = wrap_limit;
	OCR1B = int(wrap_limit * duty_cycle);

	// TCCR1A's timer mode should already be set, so just use TCCR1B to start PWM (is that sufficient?):
	TCCR1A = _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);	
	TCCR1B = (TCCR1B & TIMER_PRESCALE_MASK) | timer_prescale_bits(prescale);
}

void pwm_off() {
	TCCR1B = (TCCR1B & TIMER_PRESCALE_MASK) | TIMER_CLK_STOP;
	TCNT1 = 0;
	digitalWrite(OUTPUT_PIN, LOW); // This seems to be necessary to silence it properly (sometimes gets stuck high otherwise!)
}

void set_fan_speed(float target) {
	pwm(PWM_FREQUENCY, target);
}

void self_test() {
	party_popper_fire();
/*
	fan_speed(0.1);
	delay(2000);
	fan_speed(0.5);
	delay(5000);
//	fan_speed(1.0);
//	delay(6000);
	fan_speed(0);
	delay(6000);
*/
}

void target_pressure_increase() {
	target_pressure++;
	if (target_pressure > 1000) {target_pressure = 1000;}
}

void target_pressure_decrease() {
	target_pressure--;
	if (target_pressure < 0) {target_pressure = 0;}
}

void party_popper_fire() {
	partyPopperServo.write(PARTY_POPPER_MAX);
	digitalWrite(LED_PIN, HIGH);
	delay(1000);
	digitalWrite(LED_PIN, LOW);
	partyPopperServo.write(PARTY_POPPER_MIN);
} 


void note_on(int note, int velocity) {
	digitalWrite(LED_PIN, HIGH);
	switch (note) {
//			case FAN_NOTE_ON: control_enabled = 1; set_fan_speed(velocity / 127.0);  break;
//			case FAN_NOTE_ON: control_enabled = 1; set_fan_speed(0.4);  break;
			case FAN_NOTE_OFF: control_enabled = 0; break;
			case FAN_UP_NOTE: target_pressure_increase(); break;
			case FAN_DOWN_NOTE: target_pressure_decrease(); break;
			case PARTY_POPPER_FIRE_NOTE: party_popper_fire(); break;
		// Could also derive fan speed or target pressure (TODO!) from note velocity
	}
}

void note_off(int note, int velocity) {
	digitalWrite(LED_PIN, LOW);
}


// Code below taken from example demo code from the Sparkfun

#ifdef NOPID

void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;
  
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);  
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;
    
  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT()
{
  unsigned int ut;
  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  
  // Wait at least 4.5ms
  delay(5);
  
  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}

#endif
