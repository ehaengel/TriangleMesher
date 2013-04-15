#include <stdlib.h>
#include <stdio.h>

#include "utility.h"
#include "vector2d.h"
#include "prism.h"
#include "triangle.h"

#ifndef GEOMETRY
#define GEOMETRY

//Returns true if the two closed intervals overlap
int closed_interval_intersection(double a1, double b1, double a2, double b2);
int closed_interval_intersection(double& res_a, double& res_b, double a1, double b1, double a2, double b2);

//Returns true if the two line segments overlap
int line_segment_intersection(Vector2d v1, Vector2d v2, Vector2d w1, Vector2d w2);

//Returns true if the prism and triangle overlap
int prism_triangle_intersection(Prism p, Triangle tri);

#endif
