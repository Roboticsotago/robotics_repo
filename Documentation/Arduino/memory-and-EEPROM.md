% Arduino EEPROM (and other memory types)
% Chris Edwards \<mailto:chris.edwards@otago.ac.nz\>
% 2017-06-29

## Arduino/ATmega/AVR?!

Some preliminary notes about the relationship between Arduino, the ATmega chip family, and the AVR architecture:

* **AVR** is the processor architecture.  This is an abstract specification that defines the instruction set, memory model, etc.  AVR is an 8-bit architecture, meaning that processor registers are 8 bits in length.

* **ATmega328p, ATmega2560, etc.** are specific types of microcontroller chip made by Atmel, which implement the AVR architecture.

* **Arduino** is a family of hobbyist-friendly circuit boards that integrate an ATmega chip, USB interface, power management, and a set of header pins having a standard layout.

The AVR architecture uses the Harvard memory model (named after the US university), which has separate memory and access buses for data and program code.  This is in contrast to the Von Neumann architecture, which has a combined memory for data and code with one memory access bus.


## SI vs. binary multiplier prefixes

In the SI system, prefixes such as kilo, micro, and so on are defined in terms of powers of 10 (kilo = 1000 = `10^3` = `1e3`).  However, computer memories are usually arranged in powers of 2, so there is a family of related binary multiplier prefixes that are close to the powers of 10:

----

	kilobinary = kibi = ki = 2^10 = 1024
	megabinary = mebi = Mi = 2^20 = 1048576
	gigabinary = gibi = Gi = 2^30 = 1073741824
	terabinary = tebi = Ti = 2^40 = 1099511627776
	
----

(Note that Frink does not recognise the "kilobinary" and "ki" forms.)


## Arduino memory types, sizes, and characteristics

There are several types of memory built into the microcontroller chips used on the Arduino:

### Registers:

These are the internal working memory of the processor.  To perform operations on variables, their values must be loaded into processor registers.  The result of operations are also stored in registers prior to being copied back out to RAM.

General-purpose registers are very fast, but there are very few of them (32 in the AVR architecture), and their names are fixed (`R0`..`R31`).  Each register is 8 bits in length.

See also: <https://en.wikipedia.org/wiki/Atmel_AVR_instruction_set#Processor_registers>

When programming in assembly language, you address and manipulate registers directly and have to manage their use yourself.  When using compiled Arduino C/C++ code, you can name variables how you like and the compiler will take care of mapping them to processor registers.

### Static RAM (SRAM):

This is the working memory of your program. Values of program variables are stored here (when not being worked on by the processor), along with runtime data structures such as the stack and the heap.

The ATmega328 has 2 kiB of SRAM, and the ATmega2560 has 8 kiB.  This is a tiny amount compared to a desktop PC, which likely has over a million times as much RAM!
	
### Flash program memory:

This is where program code is stored.  When you upload a sketch, the compiled and linked binary code is written to the chip's flash memory.  The program flash memory is non-volatile, so it will remain intact when the chip is powered off.  When the chip powers up, it loads and runs whatever program is stored in flash.

The flash memory capacity is 32 kiB on the ATmega328, and 256 kiB on the ATmega2560.

### EEPROM:

This stands for Electrically Eraseable Programmable Read-Only Memory.  This memory provides a non-volatile storage that your programs can use.  It is a little like a computer's hard drive, only very basic: instead of files and folders, you are limited to reading and writing one byte at a time.  EEPROM capacity is also extremely small: the Atmega chips used on Arduino boards provide between 1 and 4 kiB of EEPROM storage.

EEPROM cells have a limited lifespan: around 100,000 write cycles before they wear out.

Arduino-based 3D printers typically use the EEPROM to store machine-specific settings such as the X, Y and Z dimensions, axis homing directions, acceleration settings, etc.  A climate-control microcontroller might use the EEPROM for storing set-points for temperature and humidity, or information about what times of the day to run the climate control system.


### Conserving SRAM

The SRAM is very limited on the basic Arduino chips, and you can easily exhaust it by storing lots of strings in your program (e.g. for debugging messages).  Your program will likely behave very strangely if it runs out of SRAM (there is no memory checking)!  A handy way to save space is to declare string constants as `PROGMEM`.  This will cause them to be stored only in program flash memory, not in SRAM.  For example:

___
```c
#include <avr/pgmspace.h>
const char GREETING[] PROGMEM = "Welcome to Awesome Firmware v1.2\
(c) 2017 The Visionaries";
// ...
	Serial.println(GREETING);
// ...
```
___

(Hmm, I tested this and it doesn't seem to work...also, the `#include` doesn't seem to be necessary.)

See also: <https://www.arduino.cc/en/Reference/PROGMEM>

Using larger data types than necessary is also a waste of memory.  If you know that a variable will never hold a value outside the range of 0..255, use a `byte` rather than an `int`.

When logging data, consider streaming the data to a larger host computer via serial or Internet, rather than storing it in SRAM.



## Using the EEPROM

Since the EEPROM capacity varies from chip to chip, it is useful to be able to find out how much is there (especially if you want to use a lot of it).  The `EEPROM.length()` function returns the number of bytes in the EEPROM.

(Hmm, only `EEPROM.length()` doesn't seem to be available in the Arduino version I'm testing on.  Apparently this got dropped when the EEPROM library was updated, and was later re-introduced.)


The Arduino API provides these low-level functions for reading/writing EEPROM data one byte at a time.  The address values are byte-level.

_____________________________________________
```c
void EEPROM.write(int address, byte value)
byte EEPROM.read(int address)
```
_____________________________________________

It also provides these higher-level functions, that allow you to read/write larger/more complex data:

___
```c
EEPROM.put(address, value)
EEPROM.get(address, value)
```
___

However, not all Arduino versions seem to support this.  If reading/writing int values (signed or unsigned---but watch for overflow!), you could use the following:

___
```c

void EEPROM_write_int(int address, int value) {
	EEPROM.write(address, highByte(value));
	EEPROM.write(address + sizeof(byte), lowByte(value));
}

int EEPROM_read_int(int address) {
	byte highByte = EEPROM.read(address);
	byte  lowByte = EEPROM.read(address + sizeof(byte));
	return word(highByte, lowByte);
}

// You could combine the two reads and the return into a one-liner:
int EEPROM_read_int(int address) {
	return word(EEPROM.read(address), EEPROM.read(address + sizeof(byte)));
}
```
___

Values of type `int` occupy two bytes, so you will need to leave room in your addressing scheme for both bytes.  There is nothing stopping you from overwriting part of an `int` in EEPROM, so be careful!  You should sketch out your EEPROM addressing scheme before implementing and using it, and document it in your program.

Since EEPROM supports only a limited number of write cycles, you should try to avoid unnecessary writes.  To help you with this, the Arduino API provides the `EEPROM.update()` function, which only writes if the data would be changing (it does a read first to see if the write is necessary):

___
```c
EEPROM.update(int address, byte value)
```
___

(This also is absent from the Arduino version I'm testing with!)

You can check the EEPROM contents for data corruption using the CRC function.  This won't let you recover or correct the data corruption---only detect it.  There is example code here:

<https://www.arduino.cc/en/Tutorial/EEPROMCrc>


## Further reading/reference:

 - <https://www.arduino.cc/en/Tutorial/EEPROM>
 - <https://www.arduino.cc/en/Reference/EEPROM>
