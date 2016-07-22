Some scripts for Arduino development without being so dependent on the (somewhat yucky) Arduino IDE.

inotool (`ino`) and its fork Arturo (`ano`) provide efficient command-line front-ends to the Arduino build tools (mainly gcc and its binutils friends plus avrdude).

I'd like to make these cross-platform (Linux, Mac OS, Windows, at least), support various front-end IDEs (Geany, Visual Studio, TextWrangler, etc.) and fall back to the Arduino IDE if necessary.


One basic script would be for taking an existing file and adjusting the path to match the Arduino IDE's requirements.
	Mac: ~/Documents/Arduino/$SKETCH/$SKETCH.ino
	Linux: 
	Windows: 

A function for determining the platform's sketchbook folder might be handy.
	On Windows, it might be a Registry entry.
	In general, it can be changed through the preferences.

	Mac default: ~/Documents/Arduino


A function for determining the location of the preferences file, e.g.
	Mac: ~/Library/Arduino15/preferences.txt
