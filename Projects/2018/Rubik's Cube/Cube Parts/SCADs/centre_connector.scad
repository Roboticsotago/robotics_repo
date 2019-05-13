include <variables.scad>

module centre() {
    difference() {
        difference() {
            union() {
                difference() {
                    difference() {
                        cubie();
                        translate([CUBIE,0,0]) icore();
                    }
                }
                rotate([0,90,0]) cylinder(CUBIE,r=INNER,center=true);
            }
            rotate([0,90,0]) {
                difference() {
                    cylinder(CUBIE+0.5,r=3,center=true);
                    translate([3-0.5+2.5,0,0]) cube([5,8,CUBIE+0.5],center=true);
                }
            }
        }
        translate([-CUBIE/2+tile_height-0.01,0]) rotate([0,90,0]) tile(tile_length*1.05,tile_height);
    }
}

module centre_connector() {
    difference() {
        difference() {
            centre();
            translate([-CUBIE/4,0,0])cube(size=[CUBIE/2,CUBIE,CUBIE],center=true);
        }
        rotate([0,90,0]) union() {
            translate([CUBIE/2-5.5,CUBIE/2-5.5,0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([-(CUBIE/2-5.5),CUBIE/2-5.5,0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([-(CUBIE/2-5.5),-(CUBIE/2-5.5),0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([CUBIE/2-5.5,-(CUBIE/2-5.5),0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
        }
    }
}
    
centre_connector();