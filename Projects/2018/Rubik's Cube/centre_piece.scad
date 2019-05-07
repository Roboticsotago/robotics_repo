include <variables.scad>
use <centre_piece_full.scad>

HOLE_RADIUS = 1.3;
HOLE_HEIGHT = 20;

module motor_connector() {
    difference() {
        center();
        translate([-CUBIE/4,0,0])cube(size=[CUBIE/2,CUBIE,CUBIE],center=true);
    }
}

module motor_connector_with_holes() {
    difference() {
        motor_connector();
        rotate([0,90,0]) union() {
            translate([CUBIE/2-5.5,CUBIE/2-5.5,0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([-(CUBIE/2-5.5),CUBIE/2-5.5,0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([-(CUBIE/2-5.5),-(CUBIE/2-5.5),0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([CUBIE/2-5.5,-(CUBIE/2-5.5),0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
        }
    }
}

motor_connector_with_holes();