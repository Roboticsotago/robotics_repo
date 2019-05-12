include <variables.scad>
use <centre_piece_full.scad>

module centre_cube() {
    difference() {
        intersection() {
            center();
            translate([-CUBIE/4,0,0])cube(size=[CUBIE/2,CUBIE,CUBIE],center=true);
        }
        union() {
            rotate([0,90,0]) tile(tile_length*1.05,100);
            rotate([90,0,0]) tile(tile_length*1.05,100);
            rotate([0,0,90]) tile(tile_length*1.05,100);
        }
    }
}

centre_cube();