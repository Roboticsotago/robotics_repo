#!/usr/bin/env tclsh8.5

set settings {
	"Sharpness" 0
	"Gain" 255
	"Exposure, Auto" 1
	"Exposure (Absolute)" 200
	"White Balance Temperature, Auto" 0
	"White Balance Temperature" 2900
}

# Ugh, uvcdynctrl doesn't seem to allow multiple -s arguments per invocation!

#set command uvcdynctrl
foreach {name value} $settings {
	puts "$name = $value"
	set command "uvcdynctrl -s \"$name\" $value"
	#puts $command
	exec {*}$command
#	if {[catch {exec uvcdynctrl -s '$name' $value} result]} {
#		puts $result
#	}

	#set command [concat $command " -s '$name' $value"]
}

#exec {*}$command
