#!/bin/bash
if [[ `hostname` == "Boris" ]]; then
	echo Goalie
	pd_patch=goalie_main.pd
else
	echo Attacker
	pd_patch=attacker_main.pd
fi
urxvt -name Pd -title Pd -e sh -c "pd -noaudio -nomidi motor-control.pd $pd_patch sensor_input.pd" &
sleep 5
v4l2-ctl -c power_line_frequency=1 -c auto_exposure=1 -c exposure_time_absolute=150 -c white_balance_auto_preset=3 -c iso_sensitivity=4 -c iso_sensitivity_auto=0
urxvt -name SimpleCV -title SimpleCV -e sh -c "python2.7 blob_tracking.py | pdsend 7001" &
urxvt -name read_sensors -title read_sensors -e sh -c "./read_sensors.tcl | pdsend 7000" &
