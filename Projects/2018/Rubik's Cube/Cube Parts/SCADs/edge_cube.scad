include <variables.scad>

module edge_cube_full() {
    difference() {
            cubie();
            translate([CUBIE,CUBIE,0])
            icore();
        }
}

module edge_cube() {                    
     difference() {
        edge_cube_full();
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
        edge_cube_full();
        translate([CUBIE/2+15,CUBIE/2+15,0]) rotate([0,0,45]) cube(size=CUBIE,center=true);
    }
}

module half_edge() {
    difference() {
        edge_cube();
        translate([0,0,CUBIE/2]) cube(size=CUBIE,center=true);
    }
}

half_edge();