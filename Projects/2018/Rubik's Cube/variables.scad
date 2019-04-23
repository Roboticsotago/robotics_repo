$fn=100;

scale_factor = 300/55;

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

core_height = 20 * scale_factor;
core_radius = 3.1 * scale_factor;

WITH_SUPPORT=false;
WITH_RAFT=false;

inner_cubie=(CUBIE-2*ROUND); //length of each cube before rounded edges

tile_length = inner_cubie*0.75;
//tile_height = 0.8 * scale_factor;
tile_height = 3;



module tile(length, height) {
	translate([-length/2,-length/2,0]) {
		minkowski(){
			translate([0,0,-height/2]) cube([length,length,height]);
			cylinder(r=ROUND,h=height,center=true);
		}
	}
	
}
//tile(tile_length,tile_height);

module cubie() {
    translate([-inner_cubie/2,-inner_cubie/2, -inner_cubie/2])
    minkowski() {
        cube([inner_cubie, inner_cubie, inner_cubie]);
        sphere(r=ROUND);
    }
}

module disc() {
    cylinder(CUBIE, r1=CORE, r2=CORE,  center=true);

}

module disc2() {
    cylinder(CUBIE*10, r1=CORE, r2=CORE,center=true);

}

module icore() {
    disc();
    rotate([0,90,0])
        disc();
    rotate([90,0,0])
        disc();
}

module core2() {
    intersection() {
        disc2();
        rotate([0,90,0])
            disc2();
        rotate([90,0,0])
            disc2();
    }
}

module screw() {
    translate([5.2,0,0])
    rotate([0,90,0]) {
        cylinder(11.7,r1=SCREW_BODY,r2=SCREW_BODY,center=true);
        translate([0,0,-11.7/2-1.7/2])
            cylinder(1.7,r1=SCREW_HEAD,r2=SCREW_HEAD,center=true);
        translate([0,0,11.7/2+5.7/2])
            cylinder(5.7,r1=SCREW_TIP,r2=SCREW_TIP,center=true);
    }
}