include <variables.scad>
use <centre_cubie.scad>
use <centre_connector.scad>

module centre_piece() {
    centre_cubie();
    centre_connector();
}