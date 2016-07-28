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

--

There are a number of hassles in getting the ino/ano folder structure to match the Arduino IDE's requirements.  Notably, the Arduino IDE will recursively try to build everything under the main folder for the sketch, so it looks like putting an ano-compatible subfolder under the main sketch folder is not an option.  I'd hoped that symlinks would allow us to keep both happy, but the Arduino IDE has problems either way:
	~/sketchbook/MySketch/MySketch.ino -> ~/sketchbook/MySketch/ano/src/sketch.ino
		Fails to build because Arduino IDE follows the symlink and ends up in the wrong dir (ano/src).
	~/sketchbook/MySketch/ano/src/sketch.ino -> ~/sketchbook/MySketch/MySketch.ino
		Fails to build in Arduino IDE because it tries to build the ano copy as well.

However, recent versions of ano support a --source-dir argument, allowing you to compile wherever the file happens to be, and some discussion on github indicates that it should now work with the Arduino IDE's folder structure.  That would make like much easier!

See:
https://github.com/scottdarch/Arturo/issues/7
