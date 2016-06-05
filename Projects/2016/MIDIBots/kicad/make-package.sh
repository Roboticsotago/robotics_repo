#!/bin/sh

# Put the Gerber files together for sending to DirtyPCBs...

BASENAME=MIDI_Shield

# ls $BASENAME*{.gto,.gts,.gtl,.gbl,.gbs,.gbo,.gml,.txt}

# Fix up file names:

mv ${BASENAME}-F.Cu.gtl ${BASENAME}.gtl
mv ${BASENAME}-B.Cu.gbl ${BASENAME}.gbl
mv ${BASENAME}-B.SilkS.gbo ${BASENAME}.gbo
mv ${BASENAME}-F.SilkS.gto ${BASENAME}.gto
mv ${BASENAME}-B.Mask.gbs ${BASENAME}.gbs
mv ${BASENAME}-F.Mask.gts ${BASENAME}.gts
mv ${BASENAME}-Edge.Cuts.gm1 ${BASENAME}.gml

# ...and the drill file:

mv ${BASENAME}.drl ${BASENAME}.txt

# Now archive them up:

#zip $BASENAME.zip $BASENAME*{.gto,.gts,.gtl,.gbl,.gbs,.gbo,.gml,.txt}
# Nope, the additional filename elements mean that won't work. And maybe `zip` doesn`t play nicely with wildcards.
#zip $BASENAME.zip $BASENAME-F.SilkS.gto $BASENAME-F.Mask.gts $BASENAME-F.Cu.gtl $BASENAME-B.Cu.gbl $BASENAME-B.Mask.gbs $BASENAME-B.SilkS.gbo $BASENAME.gml $BASENAME.txt
zip $BASENAME.zip $BASENAME.gto $BASENAME.gts $BASENAME.gtl $BASENAME.gbl $BASENAME.gbs $BASENAME.gbo $BASENAME.gml $BASENAME.txt
