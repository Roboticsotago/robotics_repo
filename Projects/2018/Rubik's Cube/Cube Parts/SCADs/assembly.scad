include <variables.scad>
include <edge piece.scad>
include <corner piece.scad>
include <centre piece.scad>

$fn=20;

a=CUBIE;
edge();
translate([-a,-a,0]) rotate([0,0,0])  edge();
translate([-a,a,0]) rotate([0,0,-90])  edge();
translate([a,a,0]) rotate([0,0,180])  edge();
translate([a,-a,0]) rotate([0,0,90])  edge();
/*translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();
translate([]) rotate([]) edge();

corner();
corner();
corner();
corner();
corner();
corner();
corner();
corner();

centre();
centre();
centre();
centre();
centre();
centre();
*/