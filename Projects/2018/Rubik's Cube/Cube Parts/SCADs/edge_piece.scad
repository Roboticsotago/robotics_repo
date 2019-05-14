include <variables.scad>
use <edge_cubie.scad>
use <edge_connector.scad>
$fn=20;

module edge_piece() {
    edge_cubie_full();
    edge_connector_full();
}

edge_piece();
