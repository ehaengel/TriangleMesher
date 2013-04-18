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
	if(v1 == 0 || v2 == 0) {
		this->v1 = 0;
		this->v2 = 0;
		return false;
	}

	if(v1 < v2) {
		this->v1 = v1;
		this->v2 = v2;
	}

	else if(v2 < v1) {
		this->v1 = v2;
		this->v2 = v1;
	}

	else {
		this->v1 = 0;
		this->v2 = 0;
		return false;
	}

	return true;
}

int Edge::SetVertex(int vertex, unsigned int vindex) {
	if(vindex == 0) {
		v1 = 0;
		v2 = 0;
		return false;
	}

	if(vertex == 0) {
		v1 = vindex;

		if(v2 != 0 && v1 > v2) {
			unsigned int buf = v1;
			v1 = v2;
			v2 = buf;
		}
	}

	else if(vertex == 1) {
		v2 = vindex;

		if(v1 != 0 && v2 < v1) {
			unsigned int buf = v1;
			v1 = v2;
			v2 = buf;
		}
	}

	else
		return false;

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


//Returns true iff both vertices are not the null vertex
int Edge::IsGoodEdge() {
	return (v1 > 0 && v2 > 0);
}
