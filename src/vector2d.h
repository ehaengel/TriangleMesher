#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <vector>
using namespace std;

#ifndef VECTOR2D
#define VECTOR2D

class Vector2d {
public:
	Vector2d();
	Vector2d(double x, double y);
	~Vector2d();

	Vector2d operator+(Vector2d v);
	Vector2d operator-(Vector2d v);
	Vector2d operator*(double r);
	Vector2d operator/(double r);

	Vector2d operator+=(Vector2d v);
	Vector2d operator-=(Vector2d v);
	Vector2d operator*=(double r);
	Vector2d operator/=(double r);

	//Dot product
	double operator*(Vector2d v);

	Vector2d operator=(Vector2d v);

	void set(double x, double y);
	void set_x(double x);
	void set_y(double y);

	double get_x();
	double get_y();
	void get(double &x, double &y);

	double mag();
	double mag2();

	double distance(Vector2d);
	double distance2(Vector2d);

	double dot(Vector2d);

	Vector2d normalize();
	Vector2d reverse();

	void print();

	double x, y;
};

typedef vector<Vector2d*> VertexList;

#endif
