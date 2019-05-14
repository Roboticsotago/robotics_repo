scale_factor = 300/55;

$fn=50;

CUBIE=18.9*scale_factor; //total side length of each cube piece (including rounded edges)
CORE=18*scale_factor;
SCREW_TIP=1.5*scale_factor;
SCREW_BODY=2*scale_factor;
SCREW_HEAD=3.8*scale_factor;
INNER=4.5*scale_factor;
SPRING=3.2*scale_factor;
ROUND=2*scale_factor; //radius of rounded edges
DEPTH=6*scale_factor;
RAFT=5*scale_factor;
LAYER=0.3*scale_factor;
GAP=0.25*scale_factor;

inner_cubie=(CUBIE-2*ROUND); //length of each cube before rounded edges

tile_length = inner_cubie*0.75;
//tile_height = 0.8 * scale_factor;
tile_height = 3;

module tile(length, height, colour) {
	color(colour)
    translate([-length/2,-length/2,0]) {
		minkowski(){
			translate([0,0,-height/2]) cube([length,length,height]);
			cylinder(r=ROUND,h=height,center=true);
		}
	}
	
}

module t(col) {
    tile(tile_length,tile_height,col);
}

module side(col) {
    color(col) translate([-tile_length-50,tile_length+50,0]) t();
    color(col) translate([0,tile_length+50,0]) t();
    color(col) translate([tile_length+50,tile_length+50,0]) t();
    
    color(col) translate([-tile_length-50,0,0]) t();
    color(col) t();
    color(col) translate([tile_length+50,0,0]) t();
    
    color(col) translate([-tile_length-50,-tile_length-50,0]) t();
    color(col) translate([0,-tile_length-50,0]) t();
    color(col) translate([tile_length+50,-tile_length-50,0]) t();
}

translate([0,0,(3*tile_length+150)/2]) side("yellow");
translate([0,0,(3*tile_length+150)/-2]) side("white");

translate([0,(3*tile_length+150)/-2,0]) rotate([90,0,0]) side("blue");
translate([0,(3*tile_length+150)/2,0]) rotate([90,0,0]) side("green");

translate([(3*tile_length+150)/2,0,0]) rotate([0,90,0]) side("red");
translate([(3*tile_length+150)/-2,0,0]) rotate([0,90,0]) side("orange");
