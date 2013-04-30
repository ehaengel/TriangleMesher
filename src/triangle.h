#include <stdlib.h>
#include <stdio.h>

#include <vector>
#include <algorithm>
using namespace std;

#include "utility.h"
#include "vector2d.h"
#include "edge.h"

#ifndef TRIANGLE
#define TRIANGLE

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
	int SetVertex(int vertex, unsigned int vindex);

	int GetVertexCount();
	unsigned int GetVertexIndex(int vertex);

	Vector2d* GetVertex(int vertex);
	Vector2d* GetGlobalVertex(unsigned int vindex);

	unsigned int GetNextVertex(unsigned int vindex);
	unsigned int GetPrevVertex(unsigned int vindex);

	int GetAdjacentVertices(int vertex, Vector2d* &v1, Vector2d* &v2);
	int GetAdjacentVerticesTriangles(int vertex, Vector2d* &v1, Vector2d* &v2, Triangle* &adj1, Triangle* &adj2);

	int IsVertex(unsigned int vindex);

	//Adjacent triangles
	int GetAdjacentTriangleCount();
	int SetAdjacentTriangle(int opposing_vertex, Triangle* tri);
	Triangle* GetAdjacentTriangle(int opposing_vertex);

	int TestAdjacency(Triangle* tri, int& opposing_vertex, int& tri_opposing_vertex);

	//Edges
	int GetOpposingEdge(Edge* &e, int opposing_vertex);
	int GetEdges(Edge* &e1, Edge* &e2, Edge* &e3);

	//Triangle index
	unsigned int GetTriangleIndex();
	int SetTriangleIndex(unsigned int triangle_index);

	//Local index
	unsigned int GetLocalIndex();
	int SetLocalIndex(unsigned int local_index);

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

	//Subdivide this triangle based on a newly added vertex
	int SubdivideTriangle(unsigned int vindex, vector<Triangle*> &results);
	int SubdivideTriangle(unsigned int vindex, vector<Triangle*> &results, double& average_new_edge_length);

	//Barycentric subdivide a triangle
	// + note that this function adds a new vertex to the global vertex list
	int BarycentricSubdivide(unsigned int& centroid_vindex, vector<Triangle*> &results);
	int BarycentricSubdivide(unsigned int& centroid_vindex, vector<Triangle*> &results, double& average_new_edge_length);

	//Subdivide along an edge
	// + the lambda determine points along the line between the two edge points
	// + if there is an adjacent triangle on the given edge it is also subdivided
	int SubdivideAlongEdge(int opposing_vertex, vector<double> lambda, vector<Triangle*> &results, vector<unsigned int> &new_vindices, double& average_new_edge_length);
	int SubdivideAlongEdge(int opposing_vertex, vector<double> lambda, vector<Triangle*> &results, vector<unsigned int> &new_vindices);

	//This is the same as above, except that it creates a uniformly spaced list of lambda values for you
	int SubdivideAlongEdge(int opposing_vertex, int count, vector<Triangle*> &results, vector<unsigned int> &new_vindices, double& average_new_edge_length);
	int SubdivideAlongEdge(int opposing_vertex, int count, vector<Triangle*> &results, vector<unsigned int> &new_vindices);

	//Compute the length of edge
	// + returns 0.0 if the edge is not well defined
	double ComputeEdgeLength(int opposing_vertex);

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

	//Triangle data
	VertexList* global_vertex_list;

	unsigned int vertices[3];
	Triangle* adjacent_triangles[3];

	Vector2d* circumcenter;
	double circumradius;

	//This is the location in the global triangle list of this triangle
	unsigned int triangle_index;

	//This is used to give each triangle a unique identifier
	static unsigned int global_triangle_count;
	unsigned int local_index;
};

typedef vector<Triangle*> TriangleList;

#endif
