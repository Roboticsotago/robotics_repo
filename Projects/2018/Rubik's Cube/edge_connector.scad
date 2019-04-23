include <variables.scad>

module edge_connector() {
    difference() {
        intersection() {
            cube([2*CUBIE-2*INNER,2*CUBIE-2*INNER,2*INNER],center=true);
                      //cube([50*scale_factor,50*scale_factor,2*INNER],center=true);
            translate([CUBIE,CUBIE,0])
            icore();
        }
        translate([(((2*CUBIE-2*INNER)/2-(CUBIE-CORE))*0.2)/2,(((2*CUBIE-2*INNER)/2-(CUBIE-CORE))*0.2)/2,0])
            scale([0.8,0.8,0.8]) 
            intersection() {
                cube([CUBIE+(CUBIE-2*INNER),CUBIE+(CUBIE-2*INNER),2*INNER],center=true);
            translate([CUBIE,CUBIE,0])
            icore();
        }
                   
     }
 }