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
v4l2-ctl -c  power_line_frequency=1 -c auto_exposure=1 -c exposure_time_absolute=150 -c white_balance_auto_preset=3 -c  iso_sensitivity=4 -c iso_sensitivity_auto=0
urxvt -name blob_tracking -title blob_tracking -e sh -c "python2.7 blob_tracking.py | pdsend 7001" &
urxvt -name read_sensors -title read_sensors -e sh -c "./read_sensors.tcl | pdsend 7000" &
