include <variables.scad>
use <edge_piece.scad>
use <corner_piece.scad>
use <centre_piece.scad>
use <core.scad>
include <test.scad>

$fn=20;

a=CUBIE;

module edges() {
    translate([-a,-a,0]) rotate([0,0,0])  edge_piece();
    translate([-a,a,0]) rotate([0,0,-90])  edge_piece();
    translate([a,a,0]) rotate([0,0,180])  edge_piece();
    translate([a,-a,0]) rotate([0,0,90])  edge_piece();
    translate([a,0,a]) rotate([-90,90,0]) edge_piece();
    translate([0,-a,a]) rotate([0,90,0]) edge_piece();
    translate([-a,0,a]) rotate([0,90,-90]) edge_piece();
    translate([0,a,a]) rotate([0,90,180]) edge_piece();
    translate([a,0,-a]) rotate([90,0,180]) edge_piece();
    translate([0,-a,-a]) rotate([0,-90,0]) edge_piece();
    translate([-a,0,-a]) rotate([90,0,0]) edge_piece();
    translate([0,a,-a]) rotate([90,0,-90]) edge_piece();
}

module corners() {
    translate([a,a,a]) rotate([0,0,180]) corner_piece();
    translate([a,-a,a]) rotate([0,0,90]) corner_piece();
    translate([-a,-a,a]) rotate([0,0,0]) corner_piece();
    translate([-a,a,a]) rotate([0,0,-90]) corner_piece();
    translate([a,a,-a]) rotate([90,0,180]) corner_piece();
    translate([a,-a,-a]) rotate([90,0,90]) corner_piece();
    translate([-a,-a,-a]) rotate([90,0,0]) corner_piece();
    translate([-a,a,-a]) rotate([90,0,-90]) corner_piece();
}

module centres() {
    translate([0,0,a]) rotate([0,90,0]) centre_piece();
    translate([0,a,0]) rotate([0,0,-90]) centre_piece();
    translate([a,0,0]) rotate([0,0,180]) centre_piece();
    translate([0,-a,0]) rotate([0,0,90]) centre_piece();
    translate([-a,0,0]) rotate([0,0,0]) centre_piece();
    translate([0,0,-a]) rotate([0,-90,0]) centre_piece();
}

core();
centres();
edges();
corners();
