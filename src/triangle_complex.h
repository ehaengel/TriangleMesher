#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "utility.h"
#include "vector2d.h"
#include "triangle.h"
#include "prism.h"

#ifndef TRIANGLE_COMPLEX
#define TRIANGLE_COMPLEX

#define MAXIMUM_MESH_SIZE	100

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

	unsigned int GetTriangleCount();
	Triangle* GetTriangle(unsigned int tindex);

	int RemoveTriangle(unsigned int tindex);
	unsigned int AppendTriangle(Triangle* tri);

	unsigned int GetVertexCount();
	unsigned int GetVertexIndex(unsigned int vertex);

	Vector2d* GetVertex(unsigned int vertex);
	Vector2d* GetGlobalVertex(unsigned int vindex);

	int SetVertexIndex(unsigned int vertex, unsigned int vindex);
	int AppendVertexIndex(unsigned int vindex);

	vector<unsigned int> GetIncompleteVertices();
	vector<TriangleList> GetIncompleteVerticesAdjacentTriangles();
	vector<TriangleEdge> GetIncompleteEdges();

	int SetIncompleteListsComputed(int incomplete_lists_computed);

	///////////////////////
	// Meshing functions //
	///////////////////////

	int RunTriangleMesher();
	int RunDelaunayFlips();

	////////////////////////////////
	// K-d tree related functions //
	////////////////////////////////

	int CreateKDTree();
	int CombineChildren();

	int SetKDParent(TriangleComplex* kd_parent);

	int SetKDSplittingDimension(int kd_splitting_dimension);
	int SetKDPrism(Prism* kd_prism);

	int SetKDLeafNodes(vector<TriangleComplex*>* kd_leaf_nodes);
	int SetKDTreePrisms(PrismList* kd_tree_prisms);

	TriangleComplex* GetKDParent();

	/////////////////////////
	// Debugging functions //
	/////////////////////////

	int write_svg(const char* filename, double w, double h);
	int write_svg(FILE* handle, double w, double h, int draw_verts);

	int generate_random_vertex_list(int num, double xmin, double xmax);

private:
	////////////////////////////
	// Internal use functions //
	////////////////////////////

	//Memory management
	int initialize();
	int free_data();

	//File reading functions
	int load_vertices(FILE* handle);
	int load_triangles(FILE* handle);

	int load_vertex_tag(char* str, Vector2d* &res);
	int load_triangle_tag(char* str, Triangle* &res);

	//Get a vertex referenced by the internal vertex list
	Vector2d* get_vertex(unsigned int index);

	//The most basic triangle mesher
	int basic_triangle_mesher();

	int create_seed_triangle();
	int compute_incomplete_vertices_and_edges();

	int is_vertex_complete(unsigned int vindex, TriangleList adjacent_triangles);

	//The most basic delaunay flipper
	int basic_delaunay_flipper();

	//Initialize the kd-tree prism based on the vertex list
	int compute_kd_prism();

	//Sort the vertex list according to either their x/y coordinate
	int sort_vertices_by_coordinate(int dim);

	//Get the index of the vertex which is center-most in a given direction
	unsigned int compute_centermost_vertex(int dim);

	//////////////////////
	// Global mesh data //
	//////////////////////

	VertexList* global_vertex_list;

	////////////////////////
	// Internal mesh data //
	////////////////////////

	//The overall mesh structure
	vector<unsigned int>* vertex_list;
	TriangleList* triangle_list;

	//These are used for constructing a mesh
	vector<unsigned int> incomplete_vertices;
	vector<TriangleList> incomplete_vertices_adjacent_triangles;

	vector<TriangleEdge> incomplete_edges;

	int incomplete_lists_computed;

	/////////////////////////////
	// K-d tree structure data //
	/////////////////////////////

	TriangleComplex* kd_parent;
	TriangleComplex* kd_child[2];

	vector<TriangleComplex*>* kd_leaf_nodes;
	PrismList* kd_tree_prisms;

	Prism* kd_prism;
	int kd_splitting_dimension;
};

#endif
