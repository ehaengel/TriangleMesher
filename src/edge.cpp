#include "edge.h"

Edge::Edge(VertexList* global_vertex_list) {
	this->global_vertex_list = global_vertex_list;

	v1 = 0;
	v2 = 0;
}

Edge::~Edge() {
	//Do nothing
}

///////////////////////////////
// Sorting related functions //
///////////////////////////////

//These are used to implement a lexicographical order on edges
int Edge::operator<=(Edge e) {
	if(v1 < e.GetVertexIndex(0))
		return true;

	else if(v1 == e.GetVertexIndex(0) && v2 <= e.GetVertexIndex(1))
		return true;

	return false;
}

int Edge::operator<(Edge e) {
	if(v1 < e.GetVertexIndex(0))
		return true;

	else if(v1 == e.GetVertexIndex(0) && v2 < e.GetVertexIndex(1))
		return true;

	return false;
}

int Edge::operator>=(Edge e) {
	return (e <= *this);
}

int Edge::operator>(Edge e) {
	return (e < *this);
}

int Edge::operator==(Edge e) {
	if(v1 == e.GetVertexIndex(0) && v2 == e.GetVertexIndex(1))
		return true;

	return false;
}

///////////////////////////////
// Data management functions //
///////////////////////////////

int Edge::SetVertices(unsigned int v1, unsigned int v2) {
	if(v1 < v2) {
		this->v1 = v1;
		this->v2 = v2;
	}

	else if(v2 < v1) {
		this->v1 = v2;
		this->v2 = v1;
	}

	else {
		v1 = 0;
		v2 = 0;
		return false;
	}

	return true;
}

unsigned int Edge::GetVertexIndex(int vertex) {
	if(vertex == 0)
		return v1;

	else if(vertex == 1)
		return v2;

	return 0;
}

Vector2d* Edge::GetVertex(int vertex) {
	return GetGlobalVertex(GetVertexIndex(vertex));
}

Vector2d* Edge::GetGlobalVertex(unsigned int vindex) {
	return (*global_vertex_list)[vindex];
}

