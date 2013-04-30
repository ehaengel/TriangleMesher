#include <stdlib.h>
#include <stdio.h>

//XML i/o code
#include "SimpleXML/src/xml_document.h"

//Triangulation algorithm related code
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

	int LoadFromFile(const char* filename, int load_triangles);
	int LoadFromFile(XML_Document* xml_document, int load_triangles);

	int SaveToFile(const char* filename, int save_triangles);
	int SaveToFile(XML_Document* xml_document, int save_triangles);

	int WriteSVG(const char* filename, double width, double height);

	///////////////////////////////
	// Data managament functions //
	///////////////////////////////

	//Free data
	int FreeAllData();
	int FreeVertexData();
	int FreeTriangleData();

	//Vertices
	unsigned int GetVertexCount();
	Vector2d* GetVertex(unsigned int index);

	int SetVertex(unsigned int index, Vector2d* vertex);
	unsigned int AppendVertex(Vector2d* vertex);

	VertexList* GetGlobalVertexList();

	//Triangles
	unsigned int GetTriangleCount();
	Triangle* GetTriangle(unsigned int index);

	int SetTriangle(unsigned int index, Triangle* tri);
	unsigned int AppendTriangle(Triangle* tri);

	int DeleteTriangle(unsigned int tindex);

	TriangleList* GetGlobalTriangleList();

private:
	////////////////////////////
	// Internal use functions //
	////////////////////////////

	int load_vertices_from_file(XML_Document* xml_document);
	int load_triangles_from_file(XML_Document* xml_document);

	//////////////////////
	// Global mesh data //
	//////////////////////

	VertexList global_vertex_list;
	TriangleList global_triangle_list;
};

#endif
