# Some stuff for testing serial comms
# Boris seems to be having some problems in that department.
# Run "rlwrap wish" and source all but the last few lines in motor-test.tcl
# Then continue in an interactive session, starting with the code below:

pack [text .out]

proc output {text} {
	.out insert end $text
}


proc kick {times} {
	if {$times <= 0} {
		return
	} else {
		incr times -1
	}
	send 00000001
	output 1
	after 100
	send 00000000
	output 0
	after 100
	
	after idle [list after 0 [list kick $times]]
}
