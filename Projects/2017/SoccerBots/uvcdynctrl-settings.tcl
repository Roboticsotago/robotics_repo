#!/usr/bin/env tclsh

# NOTE: probably don't go below Exposure=100 because power line frequency artifacts become visible (even with Power Line Frequency = 1 (50 Hz)).

# For Focus, larger values focus closer to the lens.
# White Balance Temperature: 2900 was from my testing at home
# but in the lab, under mostly fluorescent lighting, 3900 is pretty much spot on.
# Higher exposure value = brighter image

set settings {
	"Sharpness" 0
	"Gain" 20
	"Exposure, Auto" 1
	"Exposure (Absolute)" 300 
	"Focus, Auto" 0
	"Focus (absolute)" 15
	"Power Line Frequency" 1
	"White Balance Temperature, Auto" 0
	"White Balance Temperature" 3900
}
# Ugh, uvcdynctrl doesn't seem to allow multiple -s arguments per invocation!

if {$argc == 0} {
	set devicename video0
} else {
	set devicename [lindex $argv 0]
}
#set command uvcdynctrl
foreach {name value} $settings {
	puts stderr "$name = $value"
	set command "uvcdynctrl -s \"$name\" $value"
	#puts $command
	#exec {*}$command
	if {[catch {exec {*}$command} result]} {
		puts stderr $result
	}

	#set command [concat $command " -s '$name' $value"]
}

#exec {*}$command
