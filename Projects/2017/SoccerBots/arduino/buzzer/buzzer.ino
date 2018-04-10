#include "pokemon.h"
#include "nokia.h"
#include "mountainKing.h"
#include "fnaf.h"
#include "sherlock.h"

const int buzzerPin = 10;
int crotchet;

float frequency(int note) {
	return 440 * pow(2.0, (note - 69) / 12.0);
}

void playNote(int pitch, int duration) {
	tone(buzzerPin, (int)round(frequency(pitch)), duration);	
	delay(duration+5);
	noTone(buzzerPin);
	delay(5);
}

void setNoteDurations(int bpm) {
	crotchet = 60000 / bpm;
}

void playTune(float tune[][2], int tempo, int arrayLength) {
	setNoteDurations(tempo);
	for(int x=0; x<arrayLength; x+=1) {
		if(tune[x][0] == 0) {
			delay(tune[x][1]*crotchet);
		} else {
			playNote(tune[x][0], tune[x][1]*crotchet);
		}
	}
}

void setup() {
	Serial.begin(9600);
}

void loop() {
	/*playTune(pokemon, pokemonTempo, pokemonArrayLength);
    playTune(nokia, nokiaTempo, nokiaArrayLength);
    playTune(mountainKing, mountainKingTempo, mountainKingArrayLength);
    playTune(fnaf, fnafTempo, fnafArrayLength);*/
    playTune(sherlock, sherlockTempo, sherlockArrayLength);
}


