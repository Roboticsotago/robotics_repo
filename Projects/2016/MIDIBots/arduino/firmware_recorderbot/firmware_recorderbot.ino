// RecorderBot code for the 2016 MIDIBots
// Team _underscore_, Information Science Mechatronics, University of Otago

#include <MIDIBot.h>
MIDIBot recorderBot;

const int pipe1 = MOSFET_PWM_PIN;
const int pipe2 = MOSFET_2_PIN;
const int pipe3 = MOSFET_3_PIN;
const int pipe4 = MOSFET_4_PIN;

//midi note numbers
const int note1 = 60;
const int note2 = 61;
const int note3 = 62;
const int note4 = 63;

void note_on(int note, int velocity) {
    
        if(note == note1) {
          digitalWrite(pipe1, HIGH);
        } else if(note == note2) {
          digitalWrite(pipe2, HIGH);
        } else if(note == note3) {
          digitalWrite(pipe3, HIGH);
        } else if(note == note4) {
          digitalWrite(pipe4, HIGH);
        }
        digitalWrite(LED_PIN, HIGH);
       
}

void note_off(int note, int velocity) {
        if(note == note1) {
          digitalWrite(pipe1, LOW);
        } else if(note == note2) {
          digitalWrite(pipe2, LOW);
        } else if(note == note3) {
          digitalWrite(pipe3, LOW);
        } else if(note == note4) {
          digitalWrite(pipe4, LOW);
        }
        digitalWrite(LED_PIN, LOW);
}

void self_test() {
//	thisMIDIBot.test_MIDI_channel();
	// TODO: Bot-specific self-test routine...
        
        note_on(60, 100); delay(500);
        note_off(60, 100); delay(500);
        note_on(61, 100); delay(500);
        note_off(61, 100); delay(500); 
        note_on(62, 100); delay(500);
        note_off(62, 100); delay(500);
        note_on(63, 100); delay(500);
        note_off(63, 100); delay(500);
        
        note_on(60, 100); delay(3000);
        note_off(60, 100); note_on(61, 100); delay(3000);
        note_off(61, 100); note_on(62, 100); delay(3000);
        note_off(62, 100); note_on(63, 100); delay(3000);
        note_off(63, 100); delay(1000);
        
        note_on(60, 100); delay(1000);
        note_on(61, 100); delay(1000);
        note_on(62, 100); delay(1000);
        note_on(63, 100); delay(1000);  
     
        note_off(60, 100); note_off(61, 100); note_off(62, 100); note_off(63, 100);   
        
        /*for(int x=0; x<= 10; x += 1) {
          note_on(60, 100); delay(2500);
          note_off(60, 100); delay(2500);
        }*/
}

void setup() {
	//recorderBot.test_MIDI_channel();	// Indicate MIDI channel at startup
        recorderBot.begin();
}

void loop() {
        recorderBot.process_MIDI();
}
