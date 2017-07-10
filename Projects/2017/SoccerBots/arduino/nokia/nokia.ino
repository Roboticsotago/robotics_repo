//at 194bpm each beat is 0.3s

const int buzzerPin = 10;
const int quaver = 150, crotchet = 300, semibreve = 1200;


float frequency(int note) {
	return 440 * pow(2.0, (note - 69) / 12.0);
}

void playNote(int pitch, int duration) {
    tone(buzzerPin, (int)round(frequency(pitch)), duration);
    Serial.println(pitch);
    delay(duration+5);
    noTone(buzzerPin);
    delay(5);
}

void setup() {
    Serial.begin(9600);
}

void loop() {
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
