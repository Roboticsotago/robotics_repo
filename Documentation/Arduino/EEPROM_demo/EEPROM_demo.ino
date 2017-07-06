#include <EEPROM.h>

void setup() {
	Serial.begin(9600);
	Serial.println("EEPROM demo");
	
	/*
	// Not available in the Arduino version I'm using for testing.
	Serial.print("EEPROM length = ");
	Serial.print(EEPROM.length());
	Serial.println(" bytes");
	*/
	
	// Alternatively, you can also determine the EEPROM size at compile time (which relies on the board type being correctly selected when you compile):
	Serial.print("EEPROM length = ");
	Serial.print(E2END + 1);
	Serial.println(" bytes");
}

void stop() {while(1) {}}

// To write an int to EEPROM requires two write() calls, since write() works only in bytes and an int is two.
// Not sure whether to have address be a specific byte address or to multiply by sizeof(int) within the functions to create a virtual addressing scheme for int values only.
// Probably safer to leave as raw byte addresses in case we have to mix and match, but watch for accidental overwriting!

void EEPROM_write_int(int address, int value) {
	EEPROM.write(address, highByte(value));
	EEPROM.write(address + sizeof(byte), lowByte(value));
}

// Ditto for reading:

int EEPROM_read_int(int address) {
	/*
	byte highByte = EEPROM.read(address);
	byte  lowByte = EEPROM.read(address + sizeof(byte));
	return word(highByte, lowByte);
	*/
	// Or simply:
	return word(EEPROM.read(address), EEPROM.read(address + sizeof(byte)));
}

// TODO: ditto for unsigned int?  Or will this approach work for both?  Note that overflows can happen silently, e.g. 


void loop() {
	// Writing a single byte:
	EEPROM.write(0, 42);
	Serial.print(EEPROM.read(0));
	Serial.println();
	
	// Writing a multi-byte value:
	int val = 12345;
//	int val = 0;
	EEPROM.write(0,val); // NOTE: only writes the first byte!
	Serial.print(EEPROM.read(0));
	Serial.print(" ");
	Serial.print(EEPROM.read(1));
	Serial.println();
	
	// EEPROM I/O using the higher-level update() and get() functions:
	// Hmm, also not available in the Arduino version I'm using here. :(
	// Will use DIY functions for reading/writing int values instead...
	
	val = -12345;
	int out = 0;
//	EEPROM.update(0,val);
//	EEPROM.put(0,val);
	EEPROM_write_int(0, val);
	out = EEPROM_read_int(0);
//	EEPROM.get(0,out);
	Serial.print(out);
	Serial.println();

	// Dump out the entire EEPROM contents:
	for (int addr = 0; addr < E2END; addr++) {
		Serial.print(addr, HEX);
		Serial.print('	');
		Serial.println(EEPROM.read(addr), HEX);
	}
	
	stop();
}
