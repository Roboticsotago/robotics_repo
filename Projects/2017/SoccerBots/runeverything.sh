#!/bin/bash

if [[ $(hostname) == "Boris" ]]; then
	pd_patch=goalie_control_refactored.pd
	echo Goalie
else
	pd_patch=attacker_main.pd
	echo Attacker
fi

# Give the desktop some time to load:
sleep 7

# For new locally-installed Pd 0.48 (will need hcs and cyclone libraries too):
#urxvt -name Pd -title Pd -e sh -c "/usr/local/bin/pd -nomidi -alsa -noadc -r 8000 -send \"; pd dsp 1\" motor-control.pd $pd_patch sensor_input.pd" &
# For system Pd package:
urxvt -name Pd -title Pd -e sh -c "/usr/bin/pd -nomidi -alsa -noadc -r 8000 -send \"; pd dsp 1\" motor-control.pd $pd_patch sensor_input.pd" &

# Give Pd some time to start up before streaming sensor data (or TODO: replace sleep with until loop, if that makes sense):
sleep 5
urxvt -hold -name read_sensors -title read_sensors -e sh -c "./read_sensors.tcl | pdsend 7000" &
#export DISPLAY=:0.0
#./rpicam-setup.sh
#sleep 3
./rpicam-setup.sh
sleep 5
#xmessage -timeout 2 go
# What?! Do you have to move the mouse in order for the blob-tracking program to run?!
xdotool mousemove --sync 0 0
sleep 1
#xmessage go
#urxvt -hold -name not_blob_tracking -title not_blob_tracking -e sh -c "dmesg | less" &
urxvt -hold -name blob_tracking -title blob_tracking -e sh -c "python2.7 blob_tracking_cv2.py | pdsend 7001" &
#python2.7 blob_tracking.py | pdsend 7001 &
xmessage yo
