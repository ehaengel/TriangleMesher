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

//Returns true if a point lies on a line segment
int point_line_intersection(Vector2d pt, Vector2d v1, Vector2d v2) {
	double mx = v2.x - v1.x;
	double my = v2.y - v1.y;

	double n = (pt.x * (-my) + pt.y * mx) / sqrt(mx*mx + my*my);
	double s = (pt.x * mx + pt.y * my) / sqrt(mx*mx + my*my);
	if(fabs(n) < EFF_ZERO && s >= 0.0 && s <= 1.0)
		return true;

	return false;
}

//Returns true if the two line segments overlap
int line_segment_intersection(Vector2d v1, Vector2d v2, Vector2d w1, Vector2d w2) {
	Vector2d res_pt;
	return line_segment_intersection(res_pt, v1, v2, w1, w2);
}

int line_segment_intersection(Vector2d& res_pt, Vector2d v1, Vector2d v2, Vector2d w1, Vector2d w2) {
	double m1x = v2.x - v1.x;
	double m1y = v2.y - v1.y;

	double m2x = w2.x - w1.x;
	double m2y = w2.y - w1.y;

	double s_bot = -m2y * m1x + m2x * m1y;

	//If the two lines are parallel
	if(fabs(s_bot / sqrt(m2x*m2x + m2y*m2y)) < EFF_ZERO) {
		if(point_line_intersection(v1, w1, w2)) {
			res_pt = v1;
			return true;
		}
		else if(point_line_intersection(v2, w1, w2)) {
			res_pt = v2;
			return true;
		}
		else if(point_line_intersection(w1, v1, v2)) {
			res_pt = w1;
			return true;
		}
		else if(point_line_intersection(w2, v1, v2)) {
			res_pt = w2;
			return true;
		}
	}

	else {
		double dbx = v1.x - w1.x;
		double dby = v1.y - w1.y;

		double s_top = -(m2y * dbx) + (m2x * dby);
		double s = s_top / s_bot;

		if(s >= 0.0 && s <= 1.0) {
			res_pt = v1 + (v2 - v1)*s;
			return true;
		}
	}

	//There was no intersection
	res_pt.x = 0.0;
	res_pt.y = 0.0;
	return false;
}

//Returns true if the prism and triangle overlap
int prism_triangle_intersection_open(Prism p, Triangle tri) {
	bool test1, test2, test3, test4;

	//The triangle vertices
	Vector2d* tv1 = tri.GetVertex(0);
	Vector2d* tv2 = tri.GetVertex(1);
	Vector2d* tv3 = tri.GetVertex(2);
	if(tv1 == NULL || tv2 == NULL || tv3 == NULL)
		return false;

	//The prism vertices
	Vector2d pv1 = p.GetMin();
	Vector2d pv2 = Vector2d(p.GetMin().x, p.GetMax().y);
	Vector2d pv3 = p.GetMax();
	Vector2d pv4 = Vector2d(p.GetMax().x, p.GetMin().y);

	//The prism faces
	double right_x = p.GetMin().x;
	double left_x = p.GetMax().x;
	double top_y = p.GetMax().y;
	double bottom_y = p.GetMin().y;

	//Test if the right side of the prism is a splitting plane
	if(tv1->x >= right_x && tv2->x >= right_x && tv3->x >= right_x)
		return true;

	//Test if the top side of the prism is a splitting plane
	if(tv1->y >= top_y && tv2->y >= top_y && tv3->y >= top_y)
		return true;

	//Test if the left side of the prism is a splitting plane
	if(tv1->x < left_x && tv2->x <= left_x && tv3->x <= left_x)
		return true;

	//Test if the bottom side of the prism is a splitting plane
	if(tv1->y <= bottom_y && tv2->y <= bottom_y && tv3->y <= bottom_y)
		return true;

	//Test if side 0 of the triangle is a splitting plane
	if(tri.TestPointEdgeOrientation(0, pv1) >= 0 &&
		tri.TestPointEdgeOrientation(0, pv2) >= 0 &&
		tri.TestPointEdgeOrientation(0, pv3) >= 0 &&
		tri.TestPointEdgeOrientation(0, pv4) >= 0)
		return true;

	//Test if side 1 of the triangle is a splitting plane
	if(tri.TestPointEdgeOrientation(1, pv1) >= 0 &&
		tri.TestPointEdgeOrientation(1, pv2) >= 0 &&
		tri.TestPointEdgeOrientation(1, pv3) >= 0 &&
		tri.TestPointEdgeOrientation(1, pv4) >= 0)
		return true;

	//Test if side 2 of the triangle is a splitting plane
	if(tri.TestPointEdgeOrientation(2, pv1) >= 0 &&
		tri.TestPointEdgeOrientation(2, pv2) >= 0 &&
		tri.TestPointEdgeOrientation(2, pv3) >= 0 &&
		tri.TestPointEdgeOrientation(2, pv4) >= 0)
		return true;

	//No splitting plane could be found, these must overlap
	return false;
}
