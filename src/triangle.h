#include <stdlib.h>
#include <stdio.h>

#include <vector>
using namespace std;

#include "vector2d.h"
#include "utility.h"

#ifndef TRIANGLE
#define TRIANGLE

//Prototype this class
class TriangleEdge;

//Returns true if the segments v1-v2 and w1-w2 overlap
int TestLineSegmentOverlap(Vector2d v1, Vector2d v2, Vector2d w1, Vector2d w2);

class Triangle {
public:
	Triangle(VertexList* global_vertex_list);
	~Triangle();

	/////////////////////
	// Data management //
	/////////////////////

	//Vertex functions
	int SetVertex(unsigned int vindex);
	int SetVertex(int vertex, unsigned int vindex);

	int GetVertexCount();
	unsigned int GetVertexIndex(int vertex);

	Vector2d* GetVertex(int vertex);
	Vector2d* GetGlobalVertex(unsigned int vindex);

	unsigned int GetNextVertex(unsigned int vindex);
	unsigned int GetPrevVertex(unsigned int vindex);

	int IsVertex(unsigned int vindex);

	//Adjacent triangles
	int GetAdjacentTriangleCount();
	int SetAdjacentTriangle(int opposing_vertex, Triangle* tri);
	Triangle* GetAdjacentTriangle(int opposing_vertex);

	int TestAdjacency(Triangle* tri, int& opposing_vertex, int& tri_opposing_vertex);

	//////////////////////////
	// Geometric primitives //
	//////////////////////////

	//Tests if a point is inside a triangle
	int TestPointInside(Vector2d pt, bool soft_edges);

	//Tests if a point is inside the circumcircle
	int TestPointInsideCircumcircle(Vector2d pt);

	//Tests the orientation of a point with an edge
	// + Returns +1 if pt forms a ccw triangle with an edge
	// + Returns -1 if pt forms a cw triangle with an edge
	// + Returns 0 if pt is colinear with the points on an edge
	int TestPointEdgeOrientation(int opposing_vertex, Vector2d pt);

	//Tests if the triangle circumcircles overlap
	int TestCircumcircleOverlap(Triangle* tri);

	//Tests overlap using splitting planes
	int TestOverlapSplittingPlanes(Triangle* tri);

	//General test for triangle overlap
	int TestOverlap(Triangle* tri);

	//Test Delaunay condition with an adjacent triangle
	int TestDelaunay(int opposing_vertex);

	//Perform a Delaunay flip with an adjacent triangle
	int PerformDelaunayFlip(int opposing_vertex);

	//Get the circumcircle of this triangle
	int GetCircumcircle(Vector2d& center, double& radius);

	//Get the centroid of this triangle
	int GetCentroid(Vector2d& centroid);

	//Get the angle made by the edges at a given vertex
	int GetVertexAngle(int vertex, double& angle);

	//Switch around the vertices so that they are in ccw order
	int OrientVertices();

	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int print();
	int write_svg(FILE* handle, double w, double h);

private:
	////////////////////////////
	// Internal use functions //
	////////////////////////////
	int compute_circumcircle();

	Vector2d compute_centroid();

	//Triangle data
	VertexList* global_vertex_list;

	unsigned int vertices[3];
	Triangle* adjacent_triangles[3];

	Vector2d* circumcenter;
	double circumradius;
};

typedef vector<Triangle*> TriangleList;

class TriangleEdge {
public:
	TriangleEdge();
	TriangleEdge(Triangle* tri, int opposing_vertex);
	~TriangleEdge();

	int operator==(TriangleEdge te);

	TriangleEdge operator=(TriangleEdge te);

	//The definition of the edge
	Triangle* tri;
	int opposing_vertex;

	//The vertices of the edge in question
	unsigned int vertices[2];
};

#endif
