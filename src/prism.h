#include <stdlib.h>
#include <stdio.h>

#include <vector>
using namespace std;

#include "vector2d.h"
#include "utility.h"

#ifndef PRISM
#define PRISM

class Prism {
public:
	Prism();
	~Prism();

	/////////////////////
	// Data management //
	/////////////////////

	int SetMin(Vector2d min);
	int SetMax(Vector2d max);
	int SetMinMax(Vector2d min, Vector2d max);

	int SetMin(double x, double y);
	int SetMax(double x, double y);

	Vector2d GetMin();
	Vector2d GetMax();

	//////////////////////////
	// Geometric primitives //
	//////////////////////////

	//Check if the point is inside the open/closed prism
	int TestPointInside(Vector2d pt, int soft_edges);

	//Expand the prism to include this point
	int Expand(Vector2d pt);

	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int write_svg(FILE* handle, double w, double h);

private:
	Vector2d min, max;
};

typedef vector<Prism*> PrismList;

#endif
