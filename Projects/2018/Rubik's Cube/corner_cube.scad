include <variables.scad>

module corner_cube() {
    difference() {    
        difference() {
            difference() {
                cubie();
                translate([0,-CUBIE/2+tile_height,0])
                rotate(v=[1,0,0],a=90)
                tile(tile_length*1.05,tile_height*100);
            }
            translate([-CUBIE/2+tile_height,0,0])
            rotate(v=[0,1,0],a=90)
            tile(tile_length*1.05,tile_height*100);
        }
        translate([0,0,CUBIE/2-tile_height])
        tile(tile_length*1.05,tile_height*100);
    }
}

corner_cube();