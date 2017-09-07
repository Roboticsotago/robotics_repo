#!/bin/bash
if [[ `hostname` == "Boris" ]]; then
	pd_patch=goalie_control.pd
	echo Goalie
else
	pd_patch=attacker_main.pd
	echo Attacker
fi
# For new locally-installed Pd 0.48 (will need hcs and cyclone libraries too):
#urxvt -name Pd -title Pd -e sh -c "/usr/local/bin/pd -nomidi -alsa -noadc -r 8000 -send \"; pd dsp 1\" motor-control.pd $pd_patch sensor_input.pd" &
# For system Pd package:
urxvt -name Pd -title Pd -e sh -c "/usr/bin/pd -nomidi -alsa -noadc -r 8000 -send \"; pd dsp 1\" motor-control.pd $pd_patch sensor_input.pd" &
# TODO: replace sleep with until loop:
sleep 5
urxvt -name read_sensors -title read_sensors -e sh -c "./read_sensors.tcl | pdsend 7000" &
#export DISPLAY=:0.0
./rpicam-setup.sh
urxvt -name blob_tracking -title blob_tracking -e sh -c "python2.7 blob_tracking.py | pdsend 7001" &
