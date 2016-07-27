NOTE: to make these files available in the Arduino IDE Sketchbook menu, you will need to symlink the containing "arduino" folder from the sketchbook folder.

The location of the sketchbook folder is customisable, and the default location varies by platform:

Linux: ~/sketchbook
Mac OS: ~/Documents/Arduino
Windows:
	%USERPROFILE%\My Documents\Arduino
	%USERPROFILE%\Documents\Arduino
	or search the Registry for the path to "My Documents" at "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders"
	There might also be useful Registry entries created by the Arduino IDE.

It might be possible to call the Arduino IDE in command-line mode to find out the sketchbook path, although even that would vary by platform ("Arduino"/"arduino", and it might not be in the path or have a standard installation location).
