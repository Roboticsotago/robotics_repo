const int tempo = 194; //194 for nokia, 200, for pokemon
const int buzzerPin = 10;
int quaver, crotchet, minim, semibreve;

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
    quaver = crotchet * 0.5;
    minim = crotchet * 2;
    semibreve = crotchet * 4;
}

void setup() {
	Serial.begin(9600);
    setNoteDurations(tempo);
}

void nokia() {
	playNote(88, quaver);
    playNote(86, quaver);
    playNote(78, crotchet);
    playNote(80, crotchet);
     
    playNote(85, quaver);
    playNote(83, quaver);
    playNote(74, crotchet);
    playNote(76, crotchet);
     
    playNote(83, quaver);
    playNote(81, quaver);
    playNote(73, crotchet);
    playNote(76, crotchet);
     
    playNote(81, semibreve);
}

void pokemon() {
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(74, crotchet);
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(75, crotchet);
	 
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(74, crotchet);
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(73, crotchet);
	 
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(74, crotchet);
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(79, crotchet);
	 
	playNote(81, minim);
	playNote(69, minim);
	 
	playNote(77, minim);
	playNote(65, minim);
	 
	//similar to above
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(74, crotchet);
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(75, crotchet);
	 
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(74, crotchet);
	playNote(69, quaver);
	playNote(69, quaver);
	playNote(79, crotchet);
	 
	playNote(78, semibreve);
	 
	playNote(74, crotchet);
	delay(crotchet);
	delay(minim);
}

void loop() {
	pokemon();
}


