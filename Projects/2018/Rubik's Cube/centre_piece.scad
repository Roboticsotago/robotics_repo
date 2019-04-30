include <variables.scad>
use <centre_piece_full.scad>

module motor_connector() {
    difference() {
        center();
        translate([-CUBIE/4,0,0])cube(size=[CUBIE/2,CUBIE,CUBIE],center=true);
    }
}

motor_connector();