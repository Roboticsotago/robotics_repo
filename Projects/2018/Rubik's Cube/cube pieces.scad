include <self_solving_cube.scad>;

$fn=50;
CUBIE=18.9;
CORE=18;
INNER=4.5;
SCREW_TIP=1.5;
SCREW_BODY=2;
SCREW_HEAD=3.8;
SPRING=3.2;
ROUND=2;
DEPTH=6;
RAFT=5;
LAYER=0.3;
GAP=0.25;


WITH_SUPPORT=false;
WITH_RAFT=false;

inner_cubie=(CUBIE-2*ROUND);

tile_length = inner_cubie;
tile_height = 1;

module tile(length, height) {
	minkowski(){
		cube([length,length,height]);
		cylinder(r=1,h=height,center=true);
	}
	
}


module cubie() {
    translate([-inner_cubie/2,-inner_cubie/2, -inner_cubie/2])
    minkowski() {
        cube([inner_cubie, inner_cubie, inner_cubie]);
        sphere(r=ROUND, $fn=10);
    }
}


module disc() {
    cylinder(CUBIE, r1=CORE, r2=CORE, $fn=50, center=true);

}

module disc2() {
    cylinder(CUBIE*10, r1=CORE, r2=CORE, $fn=50, center=true);

}

module core() {
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
        cylinder(11.7,r1=SCREW_BODY,r2=SCREW_BODY,$fn=10,center=true);
        translate([0,0,-11.7/2-1.7/2])
            cylinder(1.7,r1=SCREW_HEAD,r2=SCREW_HEAD,$fn=10,center=true);
        translate([0,0,11.7/2+5.7/2])
            cylinder(5.7,r1=SCREW_TIP,r2=SCREW_TIP,$fn=10,center=true);
    }
}

center();

module center() 
{
	difference() {
    	difference() 
	    {
	        union() 
	        {
	            difference() 
	            {
	                cubie();
	                translate([CUBIE,0,0])
	                    core();
	
	            }
	            rotate([0,90,0])
	                cylinder(CUBIE,r1=INNER,r2=INNER,center=true);
	            if (WITH_RAFT)
	                raft();
	        }
	
	        rotate([0,90,0]) 
	        {
	            union() 
	            {
	                cylinder(CUBIE,r1=SCREW_BODY+GAP,r2=SCREW_BODY+GAP,$fn=10,center=true);
	
	                translate([0,0,CUBIE/2-1/2-3])
	                    cylinder(1.5,r1=SPRING+GAP,r2=SCREW_BODY+GAP,$fn=10,center=true);
	
	
	                translate([0,0,CUBIE/2-1/2-9.5])
	                    cylinder(1,r1=SCREW_HEAD+GAP,r2=SPRING+GAP,$fn=10,center=true);
	
	
	                translate([0,0,-4.25/2])
	                    cylinder(CUBIE-4.25,r1=SPRING+GAP,r2=SPRING+GAP,$fn=10,center=true);
	
	                translate([0,0,-10.5/2])
	                    cylinder(CUBIE-10.5,r1=SCREW_HEAD+GAP,r2=SCREW_HEAD+GAP,$fn=10,center=true); 
	            }
	        }
	    }
		translate([-inner_cubie/2 + tile_height,-inner_cubie/2,inner_cubie/2])
		rotate(v=[0,1,0], a=90)
		tile(tile_length*1.05, tile_height*1.05);		
	}
}

module edge() {
    union() {
        difference() {
            cubie();
            translate([CUBIE,CUBIE,0])
                core();
        }
          

        intersection() {
            cube([CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER),2*INNER],center=true);
            cube([50,50,2*INNER],center=true);
            translate([CUBIE,CUBIE,0])
                core();
        }
    }

    if (WITH_RAFT)
        raft();
    if (WITH_SUPPORT)
        edge_support();
}

module corner() {
    union() {
        difference() {
            cubie();
        }
 
        intersection() {
            cube([CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER)],center=true);
            translate([CUBIE,CUBIE,-CUBIE])
                core2();
        }
    }

    if (WITH_RAFT)
        raft();
    if (WITH_SUPPORT)
        corner_support();
}

module inner_cyl() {
    cylinder(CUBIE,r1=INNER,r2=INNER,center=true);
}

module inner_hole() {
    cylinder(CUBIE+0.1,r1=SCREW_TIP,r2=SCREW_TIP,$fn=10,center=true);
}

module inner_support() {
    translate([-CUBIE/4,CUBIE/2-0.75,0])
        cube([CUBIE/2,1.5,INNER],center=true);
}

module edge_support() {
    translate([0,-1.5/2+CUBIE-INNER,0])
        cube([CUBIE, 1.5, 2*INNER],center=true);

    translate([0,-1.5/2+CUBIE-INNER-2.5,0])
        cube([CUBIE, 1.5, 2*INNER],center=true);
}

module corner_support() {
    translate([0,-1.5/2+CUBIE-INNER,2*INNER-CUBIE])
        cube([CUBIE, 1.5, 2*INNER],center=true);
    rotate([-90,0,0])
        translate([0,-1.5/2+CUBIE-INNER,CUBIE-2*INNER])
            cube([CUBIE, 1.5, 2*INNER],center=true);
    translate([0,CUBIE/2+1.5,-CUBIE/2-1.5])
        cube([CUBIE, 1.5, 1.5],center=true);
}

module inner() {
    difference() {
        union() {
            inner_cyl();
            rotate([0,90,0])
                inner_cyl();
            rotate([90,0,0])
                inner_cyl();

            if (WITH_RAFT)
                raft();
            if (WITH_SUPPORT)
            {
                inner_support();
                rotate([90,0,0])
                    inner_support();
                rotate([180,0,0])
                    inner_support();
                rotate([270,0,0])
                    inner_support();
            }
        }
        union() {
            inner_hole();
            rotate([0,90,0])
                inner_hole();
            rotate([90,0,0])
                inner_hole();
        }
    }

}

module raft() {
    translate([-CUBIE/2+LAYER/2,0,0]) 
    rotate([0,90,0])
    minkowski() {
        cube([CUBIE, CUBIE, LAYER],center=true);
        cylinder(LAYER,r1=RAFT,r2=RAFT);
    }
}

module cut_through_center()
{
    intersection()
    {
        union()
        {
            center();
            screw();
        }

        translate([0,0,-CUBIE/2+LAYER/2])
            cube([100,0.1,100], center=true);
    }
}

translate([0,0,CUBIE/2])
rotate([0,-90,0])
edge(); 

translate([CUBIE+2*RAFT+2,0,CUBIE/2])
rotate([0,-90,0])
corner();

translate([2*CUBIE+4*RAFT+4,0,CUBIE/2])
rotate([0,-90,0])
center();

translate([3*CUBIE+6*RAFT+6,0,CUBIE/2])
rotate([0,-90,0])
inner();