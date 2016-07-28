#!/bin/bash

SAVE_DIR="/home/henry/Documents/robotics_repo/Projects/2016/MIDIBots/arduino/"
cd $SAVE_DIR
: '
	task:
		need to locate the sketch.ino file under each folder (*/src/sketch.ino)
		then we need to rename the sketch.ino file to the name of the folder containing the src folder.
		move the renamed file to the arduino directory of git.
		then run "arduiglue-arduino-folder-setup "%f" on the file.
		finally run "arduiglue-arturo-folder-setup "%d" on the directory
		echo the path tree of the folder in question?
'

#This is not polished it was a quick and dirty program for one time use with folder setup.
while [ 1 ]
		do
			FILE=`zenity --file-selection --directory --title="Select sketch.ino" --separator="+++"`
			parent=${FILE%/*}
			finalName=${parent##*/}   

			mv $FILE/sketch.ino $finalName.ino
			rm -R "$finalName/"
			arduiglue-arduino-folder-setup "$finalName.ino"
			arduiglue-arturo-folder-setup "$finalName/"
        done    


