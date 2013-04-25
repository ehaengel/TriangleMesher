#include <stdlib.h>
#include <stdio.h>

#include "utility.h"
#include "vector2d.h"
#include "triangle.h"

#ifndef GLOBAL_MESH_DATA
#define GLOBAL_MESH_DATA

class GlobalMeshData {
public:
	GlobalMeshData();
	~GlobalMeshData();

	//////////////
	// File i/o //
	//////////////

	int LoadFromFile(const char* filename);
	int SaveToFile(const char* filename);

	///////////////////////////////
	// Data managament functions //
	///////////////////////////////

	//Vertices
	unsigned int GetVertexCount();
	Vector2d* GetVertex(unsigned int index);

	int SetVertex(unsigned int index, Vector2d* vertex);
	unsigned int AppendVertex(Vector2d* vertex);

	//Triangles
	unsigned int GetTriangleCount();
	Triangle* GetTriangle(unsigned int index);

	int SetTriangle(unsigned int index, Triangle* tri);
	unsigned int AppendTriangle(Triangle* tri);

private:
	VertexList global_vertex_list;
	TriangleList global_triangle_list;
};

#endif
