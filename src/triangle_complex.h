#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//XML i/o code
#include "SimpleXML/src/xml_document.h"

//Triangulation algorithm related code
#include "utility.h"
#include "vector2d.h"
#include "triangle.h"
#include "prism.h"
#include "edge.h"
#include "geometry.h"

#include <omp.h>

#ifndef TRIANGLE_COMPLEX
#define TRIANGLE_COMPLEX

#define MAXIMUM_MESH_SIZE	500

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
	//int WriteToFile(FILE* handle);

	///////////////////////////////
	// Data management functions //
	///////////////////////////////

	//Data management for triangles
	unsigned int GetTriangleCount();
	Triangle* GetTriangle(unsigned int tindex);

	int SetTriangle(unsigned int tindex, Triangle* tri);
	unsigned int AppendTriangle(Triangle* tri);

	int RemoveTriangle(unsigned int tindex);
	int RemoveAllTriangles();

	int DeleteTriangle(unsigned int tindex);

	//Data management for local/global vertices
	unsigned int GetVertexCount();
	unsigned int GetVertexIndex(unsigned int vertex);

	Vector2d* GetVertex(unsigned int vertex);
	Vector2d* GetGlobalVertex(unsigned int vindex);

	int SetVertexIndex(unsigned int vertex, unsigned int vindex);
	int AppendVertexIndex(unsigned int vindex);

	//Generate some regular/random vertex sets
	int GenerateRandomGrid(double xmin, double xmax, double ymin, double ymax, unsigned int vertex_count);
	int GenerateUniformGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount);
	int GenerateHexGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount);

	//These have to do with the meshing process
	vector<unsigned int> GetIncompleteVertices();
	vector<double> GetIncompleteVerticesAngles();

	int SetIncompleteListsComputed(int incomplete_lists_computed);

	///////////////////////
	// Meshing functions //
	///////////////////////

	int RunTriangleMesher();
	int RunDelaunayFlips();

	int BasicTriangleMesher();

	int SubdivideTriangle(unsigned int vindex, unsigned int triangle_local_index);
	int BarycentricSubdivide(unsigned int triangle_local_index);

	int StretchedGridMethod(unsigned int iterations, double alpha);

	/////////////////////////////
	// Mesh Geometry functions //
	/////////////////////////////

	//This functon fills a list with triangles which overlap a prism
	int GetTrianglesInsidePrism(vector<Triangle*> &result, Prism p);

	//This function creates a list with one integer per triangle
	// + this integer is 0 if the corresponding triangle from GetTriangle(...) does not overlap p
	// + this integer is 1 if the corresponding triangle from GetTriangle(...) does overlap p
	int GetTrianglesInsidePrism(vector<int> &result, Prism p);

	//This function creates a list of edges for the whole complex
	int GetEdges(vector<Edge*> &result);

	//Compute statistics on the edges in this complex/overlapping the given prism
	// + if inside_prism is true then edges of triangles overlapping p are considered
	// + otherwise all the edges of all triangles are taken into count
	int ComputeEdgeStatistics(unsigned int& edge_count, double& avg_edge_length, int inside_prism, Prism p);

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

	int AppendBridgeTriangleIndex(unsigned int local_index);
	int IsBridgeTriangleIndex(unsigned int local_index);

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

	int reset_global_vertex_list();

	//The most basic triangle mesher
	int basic_triangle_mesher();

	int create_seed_triangle();
	int compute_incomplete_vertices();

	int is_vertex_complete(unsigned int vindex, TriangleList adjacent_triangles);

	//The most basic delaunay flipper
	int basic_delaunay_flipper();

	//The most basic stretched grid method
	int basic_stretched_grid_method(unsigned int iterations, double alpha);

	//The force based stretched grid method
	int force_stretched_grid_method(unsigned int iterations, double dt, double gamma);

	//This function cleans up the mesh to remove extra triangles
	int basic_mesh_cleaner();

	//Initialize the kd-tree prism based on the vertex list
	int compute_kd_prism();

	//Sort the vertex list according to either their x/y coordinate
	int sort_vertices_by_coordinate(int dim);

	//Get the index of the vertex which is center-most in a given direction
	unsigned int compute_centermost_vertex(int dim);

	//Figure out the adjacencies for all the triangles
	int compute_triangle_adjacencies();

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
	vector<double> incomplete_vertices_angles;

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

	vector<unsigned int> kd_bridge_triangles;
};

#endif
