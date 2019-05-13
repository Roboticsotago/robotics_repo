include <variables.scad>

HOLE_RADIUS = 1;
HOLE_HEIGHT = 20;

module centre_cubie() {
    difference() {
        intersection() {
            difference() {
                cubie();
                union() {
                    rotate([0,90,0]) tile(tile_length*1.05,100);
                    rotate([90,0,0]) tile(tile_length*1.05,100);
                    rotate([0,0,90]) tile(tile_length*1.05,100);
                }
            }
            translate([-CUBIE/4,0,0]) cube([CUBIE/2,CUBIE,CUBIE],center=true);
        }
        union() {
            rotate([0,90,0]) translate([CUBIE/2-5.5,CUBIE/2-5.5,0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            rotate([0,90,0]) translate([-(CUBIE/2-5.5),CUBIE/2-5.5,0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            rotate([0,90,0]) translate([-(CUBIE/2-5.5),-(CUBIE/2-5.5),0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            rotate([0,90,0]) translate([CUBIE/2-5.5,-(CUBIE/2-5.5),0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
        }
    }
}

centre_cubie();