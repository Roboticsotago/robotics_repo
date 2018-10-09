#!/bin/bash

if [[ $(hostname) == "Boris" ]]; then
	sleep 7
	pd_patch=goalie_control_refactored.pd
	echo Goalie
	xdotool mousemove --sync 0 0
	sleep 3
	python -u blob_tracking_cv2.py | python boris_python2.py
	xmessage yo 
else
	echo Attacker
	sleep 7
	pd_patch=attacker_main.pd
	urxvt -name Pd -title Pd -e sh -c "/usr/bin/pd -nomidi -alsa -noadc -r 8000 -send \"; pd dsp 1\" motor-control.pd $pd_patch sensor_input.pd" &
	sleep 10
	urxvt -hold -name read_sensors -title read_sensors -e sh -c "./read_sensors.tcl | pdsend 7000" &
	sleep 5
	xdotool mousemove --sync 0 0
	sleep 1
	urxvt -hold -name blob_tracking -title blob_tracking -e sh -c "python2.7 blob_tracking_cv2.py | pdsend 7001" &
	xmessage yo
fi
