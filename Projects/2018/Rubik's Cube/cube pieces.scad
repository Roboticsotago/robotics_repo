$fn=50;

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
core_radius = 3 * scale_factor;


WITH_SUPPORT=false;
WITH_RAFT=false;

inner_cubie=(CUBIE-2*ROUND); //length of each cube before rounded edges

tile_length = inner_cubie*0.75;
tile_height = 1 * scale_factor;

module tile(length, height) {
	translate([-length/2,-length/2,0]) {
		minkowski(){
			translate([0,0,-height/2]) cube([length,length,height]);
			cylinder(r=ROUND,h=height,center=true);
		}
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
        cylinder(11.7,r1=SCREW_BODY,r2=SCREW_BODY,$fn=10,center=true);
        translate([0,0,-11.7/2-1.7/2])
            cylinder(1.7,r1=SCREW_HEAD,r2=SCREW_HEAD,$fn=10,center=true);
        translate([0,0,11.7/2+5.7/2])
            cylinder(5.7,r1=SCREW_TIP,r2=SCREW_TIP,$fn=10,center=true);
    }
}


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
	                    icore();
	
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
		translate([-CUBIE/2 + tile_height/2,0])
		rotate(v=[0,1,0], a=90)
		tile(tile_length*1.05, tile_height);		
	}
}

module edge() {
	difference() {
		difference() {
	    	union() {
		        difference() {
		            cubie();
		            translate([CUBIE,CUBIE,0])
		                icore();
		        }
		          
		
		       intersection() {
		            cube([CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER),2*INNER],center=true);
		            cube([50*scale_factor,50*scale_factor,2*INNER],center=true);
		            translate([CUBIE,CUBIE,0])
		            icore();
		   	}
			}
			translate([-CUBIE/2 + tile_height/2,0,0])
			rotate(v=[0,1,0],a=90)
			tile(tile_length*1.05,tile_height);
		}
		translate([0,-CUBIE/2 + tile_height/2,0])
		rotate(v=[1,0,0],a=90)
		tile(tile_length*1.05,tile_height);
	}
	
	if (WITH_RAFT)
	    raft();
   if (WITH_SUPPORT)
	    edge_support();
}

module corner() {
	difference() {
		difference() {
			difference() {
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
				translate([0,-CUBIE/2 + tile_height/2,0])
				rotate(v=[1,0,0],a=90)
				tile(tile_length*1.05,tile_height);
			}
			translate([-CUBIE/2 + tile_height/2,0,0])
			rotate(v=[0,1,0],a=90)
			tile(tile_length*1.05,tile_height);
		}
		translate([0,0,CUBIE/2 - tile_height/2])
		tile(tile_length*1.05,tile_height);
	}
			
	if (WITH_RAFT)
   raft();
   if (WITH_SUPPORT)
   corner_support();

}

module cylinder_sphere(height) {
	intersection() {
		cylinder(h=height, r=height/2, center=true);
		rotate(a=90, v=[1,0,0]) cylinder(h=height, r=height/2, center=true);
		rotate(a=90, v=[0,1,0]) cylinder(h=height, r=height/2, center=true);
	}
}

module core_shape(height, radius) {
	cylinder(h=height,r=radius,center=true);
	rotate(a=90,v=[1,0,0]) cylinder(h=height,r=radius,center=true);
	rotate(a=90,v=[0,1,0]) cylinder(h=height,r=radius,center=true);
}

module core_filled_in() {
	core_shape(core_height, core_radius);
	cylinder_sphere(core_height*0.75);
}

module core_hollow_part() {
	core_shape(core_height, core_radius/2);
	cylinder_sphere(core_height*0.6);
}

module core() {
	difference() {
		core_filled_in();
		core_hollow_part();
	}
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

translate([470,0,core_height/2])
core();

translate([200,-150,0])
!tile(tile_length,tile_height);
