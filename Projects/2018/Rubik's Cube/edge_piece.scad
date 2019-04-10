include <variables.scad>
use <edge_cube.scad>
use <edge_connector.scad>

module edge_piece() {
    edge_cube();
    edge_connector();
}

edge_piece();