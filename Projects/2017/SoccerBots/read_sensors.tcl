#!/usr/bin/env tclsh
# Tcl serial port basics
# In Tcl, serial communication is part of a large I/O (Input/Output) framework centred around channels.  File I/O and network I/O also uses channels.

# The general procedure is to open a channel, read from and/or write to it, then close when done.
# You can configure an existing channel using the "chan configure" command.
# Tcl version on the SoccerBot Pi systems is 8.6

if {[info hostname] == "Boris"} {
	set SERIAL_DEVICE /dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_852313632363516031B2-if00
	puts stderr goalie
} else {
	set SERIAL_DEVICE /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
	puts stderr attacker
}

# Open the channel and store the channel identifier ("handle") for future reference:
puts stderr "Opening serial device $SERIAL_DEVICE ..."
set serial_channel [open $SERIAL_DEVICE RDONLY]

# Configure the channel:
chan configure $serial_channel -mode 115200,n,8,1 -translation crlf -buffering line -blocking 0

puts stderr "Good to go!"

# You can read a line of text on demand like so:
#gets $serial_channel

# A nicer way is to use Tcl's event loop, and set up an event handler that is called whenever the channel has something to read:

# Callback function:
proc read_serial channel {
	set line [gets $channel]
	if {$line != ""} {
		if {[catch {puts $line} err]} {
			puts stderr "Error write writing output: $err"
			# How to recover? Restart everything?
			# For now, just exit.
			puts stderr "Halting."
			while {1} {}
		}
		puts -nonewline stderr .	;# Indicate activity
	}
	if {[eof $channel]} {
		puts stderr "Channel closed!"
		close $channel
		set ::done true
	}
}

# Set up the callback:
chan event $serial_channel readable [list read_serial $serial_channel]

# Enter the event loop (exit when done)
vwait ::done
