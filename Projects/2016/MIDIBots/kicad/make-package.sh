#!/bin/sh

# Put the Gerber files together for sending to DirtyPCBs...

BASENAME=MIDI_Shield
ls $BASENAME*{.gto,.gts,.gtl,.gbl,.gbs,.gbo,.gml,.txt}

mv ${BASENAME}.drl ${BASENAME}.txt
mv ${BASENAME}-Edge.Cuts.gm1 ${BASENAME}.gml

#zip $BASENAME.zip $BASENAME*{.gto,.gts,.gtl,.gbl,.gbs,.gbo,.gml,.txt}
# Nope, the additional filename elements mean that won't work. And maybe `zip` doesn`t play nicely with wildcards.
zip $BASENAME.zip $BASENAME-F.SilkS.gto $BASENAME-F.Mask.gts $BASENAME-F.Cu.gtl $BASENAME-B.Cu.gbl $BASENAME-B.Mask.gbs $BASENAME-B.SilkS.gbo $BASENAME.gml $BASENAME.txt
