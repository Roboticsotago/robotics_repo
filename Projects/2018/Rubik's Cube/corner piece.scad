include <variables.scad>

$fa=1;

module corner() {
	difference() {
		difference() {
			difference() {
    			union() {
			        difference() {
			            cubie();
                        scale([0.8,0.8,0.8]) cubie();
			        }
			 
			        difference() {
                        intersection() {
                            cube([CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER)],center=true);
                            translate([CUBIE,CUBIE,-CUBIE])
                                core2();
                        }
                        
                        
                        translate([(CORE-CUBIE/2)*0.2,(CORE-CUBIE/2)*0.2,(CORE-CUBIE/2)*-0.2])
                        scale([0.8,0.8,0.8])
                        intersection() {
                            cube([CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER)],center=true);
                            translate([CUBIE,CUBIE,-CUBIE])
                                core2();
                        }
                    }

			   }
				translate([0,-CUBIE/2 + tile_height,0])
				rotate(v=[1,0,0],a=90)
				tile(tile_length*1.05,tile_height);
			}
			translate([-CUBIE/2 + tile_height,0,0])
			rotate(v=[0,1,0],a=90)
			tile(tile_length*1.05,tile_height);
		}
		translate([0,0,CUBIE/2 - tile_height])
		tile(tile_length*1.05,tile_height);
	}
			
	if (WITH_RAFT)
   raft();
   if (WITH_SUPPORT)
   corner_support();

}
/*difference(){
    corner();
    translate([-55,-55,0])cube(60);
}*/
corner();