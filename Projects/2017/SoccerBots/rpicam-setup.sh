#!/bin/sh
#config raspberry pi camera
v4l2-ctl -c power_line_frequency=1 -c iso_sensitivity_auto=0 -c auto_exposure=1 -c exposure_time_absolute=100 -c  white_balance_auto_preset=3 -c iso_sensitivity=4 -c scene_mode=0
