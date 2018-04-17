include <variables.scad>

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

edge();