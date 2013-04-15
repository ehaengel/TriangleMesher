#include "geometry.h"

//Returns true if the two closed intervals overlap
int closed_interval_intersection(double a1, double b1, double a2, double b2) {
	if(a1 <= a2 && b1 >= a2 && b1 <= b2)
		return true;

	else if(a1 <= b2 && b1 >= b2 && a1 >= a2)
		return true;

	else if(a1 >= a2 && b1 <= b2)
		return true;

	else if(a1 < a2 && b1 > b2)
		return true;

	return false;
}

int closed_interval_intersection(double& res_a, double& res_b, double a1, double b1, double a2, double b2) {
	res_a = 0.0;
	res_b = 0.0;

	if(a1 <= a2 && b1 >= a2 && b1 <= b2) {
		res_a = a2;
		res_b = b1;

		return true;
	}
	else if(a1 <= b2 && b1 >= b2 && a1 >= a2) {
		res_a = a1;
		res_b = b2;

		return true;
	}
	else if(a1 >= a2 && b1 <= b2) {
		res_a = a1;
		res_b = b1;

		return true;
	}
	else if(a1 < a2 && b1 > b2) {
		res_a = a2;
		res_b = b2;

		return true;
	}

	return false;
}

//Returns true if the two line segments overlap
int line_segment_intersection(Vector2d v1, Vector2d v2, Vector2d w1, Vector2d w2) {


	return false;
}

//Returns true if the prism and triangle overlap
int prism_triangle_intersection(Prism p, Triangle tri) {

	return false;
}

