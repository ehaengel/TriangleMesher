#include <stdlib.h>
#include <stdio.h>

#include <vector>
using namespace std;

#include "vector2d.h"
#include "utility.h"

#ifndef TRIANGLE
#define TRIANGLE

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

	unsigned int GetNextVertex(unsigned int vindex);
	unsigned int GetPrevVertex(unsigned int vindex);

	//Adjacent triangles
	int GetAdjacentTriangleCount();
	int SetAdjacentTriangle(int vertex, Triangle* T);
	Triangle* GetAdjacentTriangle(int vertex);

	//////////////////////////
	// Geometric primitives //
	//////////////////////////

	//Tests if a point is inside a triangle
	int TestPointInside(Vector2d pt, bool soft_edges);

	//Tests the orientation of a point with an edge
	// + Returns +1 if pt forms a ccw triangle with an edge
	// + Returns -1 if pt forms a cw triangle with an edge
	// + Returns 0 if pt is colinear with the points on an edge
	int ComputePointEdgeOrientation(int opposing_vertex, Vector2d pt);

	//Switch around the vertices so that they are in ccw order
	int OrientVertices();

	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int print();
	int write_svg(FILE* handle);

private:
	////////////////////////////
	// Internal use functions //
	////////////////////////////

	//Triangle data
	VertexList* global_vertex_list;

	unsigned int vertices[3];
	Triangle* adjacent_triangles[3];
};

typedef vector<Triangle*> TriangleList;

#endif
