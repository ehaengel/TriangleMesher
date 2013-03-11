#include <stdlib.h>
#include <stdio.h>

#include <map>
#include <vector>
using namespace std;

#include "utility.h"
#include "vector2d.h"
#include "triangle.h"

#ifndef TRIANGLE_COMPLEX
#define TRIANGLE_COMPLEX

#define MAXIMUM_MESH_SIZE	1e6

class TriangleComplex {
public:
	TriangleComplex();
	TriangleComplex(VertexList* global_vertex_list);

	~TriangleComplex();

	///////////////////////////////
	// General use i/o functions //
	///////////////////////////////

	int LoadFromFile(const char* filename);
	int WriteToFile(const char* filename);
	int WriteToFile(FILE* handle);

	///////////////////////////////
	// Data management functions //
	///////////////////////////////


	///////////////////////
	// Meshing functions //
	///////////////////////

	int RunTriangleMesher();
	int RunDelaunayFlips();


	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int write_svg(const char* filename, double w, double h);
	int write_svg(FILE* handle, double w, double h);

	int generate_random_vertex_list(int num, double xmin, double xmax);

private:
	////////////////////////////
	// Internal use functions //
	////////////////////////////

	//Get a vertex referenced by the internal vertex list
	Vector2d* get_vertex(unsigned int index);

	//The most basic triangle mesher
	int basic_triangle_mesher();

	int create_seed_triangle();
	int compute_incomplete_vertices_and_edges();

	int is_vertex_complete(unsigned int vindex, TriangleList adjacent_triangles);

	//////////////////////
	// Global mesh data //
	//////////////////////

	VertexList* global_vertex_list;

	////////////////////////
	// Internal mesh data //
	////////////////////////

	//The overall mesh structure
	vector<unsigned int> vertex_list;
	TriangleList* triangle_list;


	//These are used for constructing a mesh
	vector<unsigned int> incomplete_vertices;
	vector<TriangleList> incomplete_vertices_adjacent_triangles;

	map<unsigned int, int> incomplete_edges;
};

#endif
