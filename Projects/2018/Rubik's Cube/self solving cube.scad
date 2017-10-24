$fn = 30;

scale_factor = 300/55;
core_height = 20 * scale_factor;
core_radius = 3 * scale_factor;


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


difference() {
	core();
	cube([150,150,150]);
}


