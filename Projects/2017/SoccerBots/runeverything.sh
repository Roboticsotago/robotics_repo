#!/bin/bash
if [[ `hostname` == "Boris" ]]; then
	pd_patch=goalie_control.pd
	echo Goalie
else
	pd_patch=attacker_main.pd
	echo Attacker
fi
urxvt -name Pd -title Pd -e sh -c "pd -noaudio -nomidi -send \"; pd dsp 1\" motor-control.pd $pd_patch sensor_input.pd" &
# TODO: replace sleep with until loop:
sleep 5
urxvt -name read_sensors -title read_sensors -e sh -c "./read_sensors.tcl | pdsend 7000" &
#export DISPLAY=:0.0
./rpicam-setup.sh
urxvt -name blob_tracking -title blob_tracking -e sh -c "python2.7 blob_tracking.py | pdsend 7001" &
