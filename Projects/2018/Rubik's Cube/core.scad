include <variables.scad>

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
    cube([radius,radius,height],center=true);
	rotate(a=90,v=[1,0,0]) cube([radius,radius,height],center=true);
	rotate(a=90,v=[0,1,0]) cube([radius,radius,height],center=true);
}

module core_filled_in() {
	core_shape(core_height, core_radius);
	cylinder_sphere(core_height*0.75);
}


module core_hollow_part() {
	core_shape(core_height, core_radius*0.9);
	cylinder_sphere(core_height*0.7);
}

module core() {
	difference() {
		core_filled_in();
		core_hollow_part();
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



core_half();