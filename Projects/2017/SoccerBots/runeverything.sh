#!/bin/bash
if [[ `hostname` == "Boris" ]]; then
	pd_patch=goalie_main.pd
	echo Goalie
else
	pd_patch=attacker_main.pd
	echo Attacker
fi
pd -noaudio -nomidi motor-control.pd $pd_patch sensor_input.pd &
sleep 5
v4l2-ctl -c  power_line_frequency=1 -c auto_exposure=1 -c exposure_time_absolute=150 -c white_balance_auto_preset=3 -c  iso_sensitivity=4 -c iso_sensitivity_auto=0
python2.7 blob_tracking.py | pdsend 7001 & 
./read_sensors.tcl | pdsend 7000 &
