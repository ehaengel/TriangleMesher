#include "vector2d.h"

Vector2d::Vector2d() {
	x = 0.0;
	y = 0.0;
}

Vector2d::Vector2d(double x, double y) {
	this->x = x;
	this->y = y;
}

Vector2d::~Vector2d() {
}

Vector2d Vector2d::operator+(Vector2d v) {
	return Vector2d(x + v.x, y + v.y);
}

Vector2d Vector2d::operator-(Vector2d v) {
	return Vector2d(x - v.x, y - v.y);
}

Vector2d Vector2d::operator*(double r) {
	return Vector2d(r*x, r*y);
}

Vector2d Vector2d::operator/(double r) {
	return Vector2d(x/r, y/r);
}

Vector2d Vector2d::operator+=(Vector2d v) {
	x += v.x;
	y += v.y;
	return (*this);
}

Vector2d Vector2d::operator-=(Vector2d v) {
	x -= v.x;
	y -= v.y;
	return (*this);
}

Vector2d Vector2d::operator*=(double r) {
	x *= r;
	y *= r;
	return (*this);
}

Vector2d Vector2d::operator/=(double r) {
	x /= r;
	y /= r;
	return (*this);
}

double Vector2d::operator*(Vector2d v) {
	return (x*v.x + y*v.y);
}

Vector2d Vector2d::operator=(Vector2d v) {
	x = v.x;
	y = v.y;
	return (*this);
}

void Vector2d::set(double x, double y) {
	this->x = x;
	this->y = y;
}

void Vector2d::set_x(double x) {
	this->x = x;
}

void Vector2d::set_y(double y) {
	this->y = y;
}

double Vector2d::get_x() {
	return x;
}

double Vector2d::get_y() {
	return y;
}

void Vector2d::get(double &x, double &y) {
	x = this->x;
	y = this->y;
}

double Vector2d::mag() {
	return sqrt(x*x + y*y);
}

double Vector2d::mag2() {
	return x*x + y*y;
}

double Vector2d::distance(Vector2d v) {
	double dx = (x - v.x);
	double dy = (y - v.y);
	return sqrt(dx*dx + dy*dy);
}

double Vector2d::distance2(Vector2d v) {
	double dx = (x - v.x);
	double dy = (y - v.y);
	return (dx*dx + dy*dy);
}

double Vector2d::dot(Vector2d v) {
	return (x*v.x + y*v.y);
}

void Vector2d::print() {
	printf("<%f, %f>\n", x, y);
}

Vector2d Vector2d::normalize() {
	double temp = mag();
	x /= temp;
	y /= temp;
	return (*this);
}

Vector2d Vector2d::reverse() {
	x = -x;
	y = -y;
	return (*this);
}
