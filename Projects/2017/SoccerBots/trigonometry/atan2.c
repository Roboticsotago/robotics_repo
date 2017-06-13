// Demonstrate/test the use of the two-argument atan2() function

// The atan2(x,y) function has a couple of advantages over conventional atan(), namely it avoids the division-by-zero problem, and returns values that correctly indicate the quadrant of the angle, based on the sign* of the inputs.
// * not the sin()!

#include <math.h>
#include <stdio.h>

const double TAU = asin(1) * 4.0;

double degrees(double radians) {return 360.0 / TAU * radians;}

void main() {
	for (int x = -1; x < 2; x++) {
		for (int y = -1; y < 2; y++) {
			printf("atan2(%i,%i) = %f\n", x, y, degrees(atan2(x,y)));
		}
	}
}
