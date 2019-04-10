include <variables.scad>

module edge_cube() {                    
     difference() {
        difference() {
            cubie();
            translate([CUBIE,CUBIE,0])
            icore();
        }
        union() {
            translate([0,-CUBIE/2 + tile_height,0])
            rotate(v=[1,0,0],a=90)
            tile(tile_length*1.05,tile_height+500);
            
            translate([-CUBIE/2 + tile_height,0,0])
			rotate(v=[0,1,0],a=90)
			tile(tile_length*1.05,tile_height+500);
        }
    }
}

edge_cube();