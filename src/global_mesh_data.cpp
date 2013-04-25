#include "global_mesh_data.h"

GlobalMeshData::GlobalMeshData() {
	global_vertex_list.clear();
	global_triangle_list.clear();

	global_vertex_list.push_back(NULL);
	global_triangle_list.push_back(NULL);
}

GlobalMeshData::~GlobalMeshData() {
	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* v = GetVertex(i);
		if(v != NULL)
			delete v;
	}

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri != NULL)
			delete tri;
	}
}

//////////////
// File i/o //
//////////////

int GlobalMeshData::LoadFromFile(const char* filename) {

	return true;
}

int GlobalMeshData::SaveToFile(const char* filename) {

	return true;
}

///////////////////////////////
// Data managament functions //
///////////////////////////////

//Vertices
unsigned int GlobalMeshData::GetVertexCount() {
	return global_vertex_list.size();
}

Vector2d* GlobalMeshData::GetVertex(unsigned int index) {
	if(index >= GetVertexCount())
		return NULL;

	return global_vertex_list[index];
}

int GlobalMeshData::SetVertex(unsigned int index, Vector2d* vertex) {
	if(index >= GetVertexCount())
		return false;

	global_vertex_list[index] = vertex;
	return true;
}

unsigned int GlobalMeshData::AppendVertex(Vector2d* vertex) {
	global_vertex_list.push_back(vertex);
	return (global_vertex_list.size()-1);
}

//Triangles
unsigned int GlobalMeshData::GetTriangleCount() {
	return global_triangle_list.size();
}

Triangle* GlobalMeshData::GetTriangle(unsigned int index) {
	if(index >= GetTriangleCount())
		return NULL;

	return global_triangle_list[index];
}

int GlobalMeshData::SetTriangle(unsigned int index, Triangle* tri) {
	if(index >= GetTriangleCount())
		return false;

	global_triangle_list[index] = tri;
	return true;
}

unsigned int GlobalMeshData::AppendTriangle(Triangle* tri) {
	global_triangle_list.push_back(tri);
	return (global_triangle_list.size()-1);
}
