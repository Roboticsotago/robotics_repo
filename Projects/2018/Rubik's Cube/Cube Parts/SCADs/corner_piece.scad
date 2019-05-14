include <variables.scad>
use <corner_cubie.scad>
use <corner_connector.scad>

module corner_piece() {
    full_corner_cubie();
    corner_connector();
}
corner_piece();