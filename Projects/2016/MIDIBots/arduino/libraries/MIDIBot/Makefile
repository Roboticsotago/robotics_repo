# TODO: make platform-aware:
# see http://stackoverflow.com/questions/714100/os-detecting-makefile

# Mac OS:
#SKETCHBOOK=~/Documents/Arduino/

# Linux (recent Arduino versions):
SKETCHBOOK=~/Arduino
# (NOTE: earlier versions had ~/sketchbook)

TARGET=${SKETCHBOOK}/libraries/MIDIBot

install: MIDIBot.h MIDIBot.cpp
	mkdir -p ${TARGET}
	cp -v MIDIBot.h MIDIBot.cpp ${TARGET}
