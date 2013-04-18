#include "prism.h"

Prism::Prism() {
	min = Vector2d(0.0, 0.0);
	max = Vector2d(0.0, 0.0);
}

Prism::~Prism() {
	//Do nothing
}

int Prism::SetMin(Vector2d min) {
	this->min = min;
	return true;
}

int Prism::SetMax(Vector2d max) {
	this->max = max;
	return true;
}

int Prism::SetMinMax(Vector2d min, Vector2d max) {
	this->min = min;
	this->max = max;

	return true;
}

int Prism::SetMin(double x, double y) {
	this->min = Vector2d(x, y);
	return true;
}

int Prism::SetMax(double x, double y) {
	this->max = Vector2d(x, y);
	return true;
}

Vector2d Prism::GetMin() {
	return min;
}

Vector2d Prism::GetMax() {
	return max;
}

//Check if the point is inside the open/closed prism
int Prism::TestPointInside(Vector2d pt, int soft_edges) {
	//Test if the point is inside the open prism
	if(soft_edges == true) {
		if(pt.x <= min.x - EFF_ZERO || pt.x >= max.x + EFF_ZERO)
			return false;

		if(pt.y <= min.y - EFF_ZERO || pt.y >= max.y + EFF_ZERO)
			return false;
	}

	//Test if the point is inside the closed prism
	else {
		if(pt.x < min.x - EFF_ZERO || pt.x > max.x + EFF_ZERO)
			return false;

		if(pt.y < min.y - EFF_ZERO || pt.y > max.y + EFF_ZERO)
			return false;
	}

	//The point was inside the prism
	return true;
}

//Expand the prism to include this point
int Prism::Expand(Vector2d pt) {
	if(pt.x < min.x)
		min.x = pt.x;

	if(pt.y < min.y)
		min.y = pt.y;

	if(pt.x > max.x)
		max.x = pt.x;

	if(pt.y > max.y)
		max.y = pt.y;

	return true;
}

int Prism::write_svg(FILE* handle, double w, double h) {
	double x = min.x;
	double y = h - max.y;

	double width = (max.x - min.x);
	double height = (max.y - min.y);

	fprintf(handle, "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" ", x, y, width, height);
	fprintf(handle, "fill=\"none\" stroke-width=\"5\" stroke=\"red\"/>\n");

	return true;
}
