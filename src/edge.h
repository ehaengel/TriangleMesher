#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <vector>
using namespace std;

#include "utility.h"
#include "vector2d.h"

#ifndef EDGE
#define EDGE

class Edge {
public:
	Edge(VertexList* global_vertex_list);
	~Edge();

	///////////////////////////////
	// Sorting related functions //
	///////////////////////////////

	//These are used to implement a lexicographical order on edges
	int operator<=(Edge e);
	int operator<(Edge e);
	int operator>=(Edge e);
	int operator>(Edge e);
	int operator==(Edge e);

	///////////////////////////////
	// Data management functions //
	///////////////////////////////

	int SetVertices(unsigned int v1, unsigned int v2);

	unsigned int GetVertexIndex(int vertex);
	Vector2d* GetVertex(int vertex);

	Vector2d* GetGlobalVertex(unsigned int vindex);

private:
	//The two vertices for this edge
	// + these are always represented with v1 < v2
	unsigned int v1, v2;

	//The global list of all vertices
	VertexList* global_vertex_list;
};

typedef vector<Edge*> EdgeList;

#endif
