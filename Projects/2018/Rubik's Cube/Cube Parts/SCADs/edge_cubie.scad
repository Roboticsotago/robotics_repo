include <variables.scad>

module edge_cube_filled() {
    difference() {
            cubie();
            translate([CUBIE,CUBIE,0])
            icore();
        }
}

module edge_cubie_full() {                    
     difference() {
        edge_cube_filled();
        union() {
            translate([0,-CUBIE/2 + tile_height,0])
            rotate(v=[1,0,0],a=90)
            tile(tile_length*1.05,tile_height+500);
            
            translate([-CUBIE/2 + tile_height,0,0])
			rotate(v=[0,1,0],a=90)
			tile(tile_length*1.05,tile_height+500);
        }
    }
    intersection() {
        edge_cube_filled();
        translate([CUBIE/2+15,CUBIE/2+15,0]) rotate([0,0,45]) cube(size=CUBIE,center=true);
    }
}

module edge_cubie() {
    difference() {
        difference() {
            edge_cubie_full();
            translate([0,0,CUBIE/2]) cube(size=CUBIE,center=true);
        }
        union() {
            translate([CUBIE/2-20,CUBIE/2-20,0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([-(CUBIE/2-5.5),CUBIE/2-5.5,0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([-(CUBIE/2-5.5),-(CUBIE/2-5.5),0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
            translate([CUBIE/2-5.5,-(CUBIE/2-5.5),0]) cylinder(r=HOLE_RADIUS, h=HOLE_HEIGHT, center=true);
        } 
    }
}

edge_cubie();