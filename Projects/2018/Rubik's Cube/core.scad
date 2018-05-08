include <variables.scad>

//$fa=1;
$fn=100;
SQUARE_THICKNESS=2;

module cylinder_sphere(height) {
	intersection() {
		cylinder(h=height, r=height/2, center=true);
		rotate(a=90, v=[1,0,0]) cylinder(h=height, r=height/2, center=true);
		rotate(a=90, v=[0,1,0]) cylinder(h=height, r=height/2, center=true);
	}
}

module core_shape_cylinders(height, radius) {
	cylinder(h=height,r=radius,center=true);
	rotate(a=90,v=[1,0,0]) cylinder(h=height,r=radius,center=true);
	rotate(a=90,v=[0,1,0]) cylinder(h=height,r=radius,center=true);
}

module core_shape(height, radius){
    cube([radius*2,radius*2,height],center=true);
	rotate(a=90,v=[1,0,0]) cube([radius*2,radius*2,height],center=true);
	rotate(a=90,v=[0,1,0]) cube([radius*2,radius*2,height],center=true);
}

module core_filled_in() {
	core_shape(core_height, core_radius);
	cylinder_sphere(core_height*0.75);
}


module core_hollow_part() {
	core_shape(core_height, core_radius*0.9);
	cylinder_sphere(core_height*0.7);
}

module square_bits() {
     translate([0,0,core_height/2-(SQUARE_THICKNESS/2)]) cube([core_radius*2,core_radius*2,SQUARE_THICKNESS],center=true);
    translate([0,0,-core_height/2+(SQUARE_THICKNESS/2)]) cube([core_radius*2,core_radius*2,SQUARE_THICKNESS],center=true);
    
    translate([0,core_height/2-(SQUARE_THICKNESS/2),0]) rotate([90,0,0]) cube([core_radius*2,core_radius*2,SQUARE_THICKNESS],center=true);
    translate([0,-core_height/2+(SQUARE_THICKNESS/2),0]) rotate([90,0,0])cube([core_radius*2,core_radius*2,SQUARE_THICKNESS],center=true);
    
    translate([core_height/2-(SQUARE_THICKNESS/2),0,0]) rotate([0,90,0]) cube([core_radius*2,core_radius*2,SQUARE_THICKNESS],center=true);
    translate([-core_height/2+(SQUARE_THICKNESS/2),0,0]) rotate([0,90,0]) cube([core_radius*2,core_radius*2,SQUARE_THICKNESS],center=true);
}

module screw_holes() {
    translate([23/2,23/2,0]) cylinder(h=core_height+5,r=3.5/2,center=true);
    translate([23/2,-23/2,0]) cylinder(h=core_height+5,r=3.5/2,center=true);
    translate([-23/2,-23/2,0]) cylinder(h=core_height+5,r=3.5/2,center=true);
    translate([-23/2,23/2,0]) cylinder(h=core_height+5,r=3.5/2,center=true);
    cylinder(h=core_height+5,r=23/2,center=true);
}

module core() {
	difference(){
        difference() {
            difference() {
                union() {
                    difference() {
                        core_filled_in();
                        core_hollow_part();
                    }
                    square_bits();
                }
                
                screw_holes();
            }
            rotate([90,0,0]) screw_holes();
        }
        rotate([0,90,0]) screw_holes();
    }
}

/*
code to show the core and the inside of the core

difference() {
    core();
    cube([core_height/2,core_height/2,core_height/2]);
}*/

module core_half() {
    difference() {
        core();
        translate([0,0,-core_height/4])cube([core_height,core_height,core_height/2],center=true);
    }
}



core();