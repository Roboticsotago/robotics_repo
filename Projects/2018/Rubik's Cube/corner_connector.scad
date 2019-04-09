include <variables.scad>
use <corner_cube.scad>

module corner_piece_connector_solid() {
    difference() {
        intersection() {
        cube(size=CUBIE+CUBIE-2*INNER,center=true);
        translate([CUBIE,CUBIE,-CUBIE])
        core2();
        }
    }
}
module corner_connector() {
    difference() {
        corner_piece_connector_solid();
        cube(size=CUBIE, center=true);
    }
}

corner_connector();
