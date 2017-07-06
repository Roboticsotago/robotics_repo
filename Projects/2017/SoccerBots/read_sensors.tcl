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


# A nicer way is to use Tcl's event loop, and set up an event handler that is called whenever the channel has something to read:

# Callback function:
proc read_serial channel {
	global SERIAL_DEVICE
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
		puts stderr "Channel EOF! Will try to reconnect..."
		close $channel
		connect $SERIAL_DEVICE
	}
}


proc connect {serial_device} {
	# Open the channel and store the channel identifier ("handle") for future reference:
	puts stderr "Opening serial device ${serial_device}..."
	# Loop for reliability/reconnection capability:
	while {1} {
		catch {set ::serial_channel [open $serial_device RDONLY]} result options
		if {[dict get $options -code] == 0} {
			puts stderr "Opened OK."
			break
		}
		puts stderr "$result"
		puts stderr "Retrying in 1 s..."
		after 1000
	}

	# Configure the channel:
	chan configure $::serial_channel -mode 115200,n,8,1 -translation crlf -buffering line -blocking 0

	# Set up the callback:
	chan event $::serial_channel readable [list read_serial $::serial_channel]

	puts stderr "Good to go!"
}


connect $SERIAL_DEVICE

# Enter the event loop (exit when done)
vwait forever
