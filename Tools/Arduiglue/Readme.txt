Some scripts for Arduino development without being so dependent on the (somewhat yucky) Arduino IDE.

inotool (`ino`) and its fork Arturo (`ano`) provide efficient command-line front-ends to the Arduino build tools (mainly gcc and its binutils friends plus avrdude).

I'd like to make these cross-platform (Linux, Mac OS, Windows, at least), support various front-end IDEs (Geany, Visual Studio, popular text editors) and fall back to the Arduino IDE if necessary.

