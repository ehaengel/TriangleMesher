#include "triangle.h"

Triangle::Triangle(VertexList* global_vertex_list) {
	this->global_vertex_list = global_vertex_list;

	vertices[0] = 0;
	vertices[1] = 0;
	vertices[2] = 0;

	adjacent_triangles[0] = NULL;
	adjacent_triangles[1] = NULL;
	adjacent_triangles[2] = NULL;
}

Triangle::~Triangle() {
	//Do nothing
}

//Data management
int Triangle::SetVertex(unsigned int vindex) {
	//Vertex 0 is the null vertex
	if(vertices[0] == 0)
		SetVertex(0, vindex);

	//Vertex 1 is the null vertex
	else if(vertices[1] == 0)
		SetVertex(1, vindex);

	//Vertex 2 is the null vertex
	else if(vertices[2] == 0)
		SetVertex(2, vindex);

	//All the vertices are filled
	else
		return false;

	return true;
}

int Triangle::SetVertex(int vertex, unsigned int vindex) {
	//Safety check
	if(vertex < 0 || vertex > 2)
		return false;

	vertices[vertex] = vindex;
	return true;
}

int Triangle::GetVertexCount() {
	int count = 0;
	for(int i=0; i<3; i++)
		if(vertices[i] != 0)
			count++;

	return count;
}

unsigned int Triangle::GetVertexIndex(int vertex) {
	//Safety check
	if(vertex < 0 || vertex > 2)
		return 0;

	return vertices[vertex];
}

Vector2d* Triangle::GetVertex(int vertex) {
	//Safety check
	if(vertex < 0 || vertex > 2)
		return NULL;

	return (*global_vertex_list)[vertices[vertex]];
}

unsigned int Triangle::GetNextVertex(unsigned int vindex) {
	//This function looks through vertices to find vindex and then
	//returns the next vertex in vertices that comes after it

	if(vertices[0] == vindex)
		return vertices[1];

	else if(vertices[1] == vindex)
		return vertices[2];

	else if(vertices[2] == vindex)
		return vertices[0];

	//If vindex is not found return the null vertex
	return 0;
}

unsigned int Triangle::GetPrevVertex(unsigned int vindex) {
	//This function looks through vertices to find vindex and then
	//returns the next vertex in vertices that comes before it

	if(vertices[0] == vindex)
		return vertices[2];

	else if(vertices[1] == vindex)
		return vertices[0];

	else if(vertices[2] == vindex)
		return vertices[1];

	//If vindex is not found return the null vertex
	return 0;
}

int Triangle::GetAdjacentTriangleCount() {
	int count = 0;

	for(int i=0; i<3; i++)
		if(adjacent_triangles[i] != NULL)
			count++;

	return count;
}

int Triangle::SetAdjacentTriangle(int vertex, Triangle* T) {
	adjacent_triangles[vertex] = T;

	return true;
}

Triangle* Triangle::GetAdjacentTriangle(int vertex) {
	return adjacent_triangles[vertex];
}

//Geometric primitives
int Triangle::TestPointInside(Vector2d pt, bool soft_edges) {
	//Checks if point is inside the open triangle
	if(soft_edges) {
		if(ComputePointEdgeOrientation(0, pt) <= 0)
			return false;

		else if(ComputePointEdgeOrientation(1, pt) <= 0)
			return false;

		else if(ComputePointEdgeOrientation(2, pt) <= 0)
			return false;
	}

	//Checks if point is inside the closed triangle
	else {
		if(ComputePointEdgeOrientation(0, pt) == -1)
			return false;

		else if(ComputePointEdgeOrientation(1, pt) == -1)
			return false;

		else if(ComputePointEdgeOrientation(2, pt) == -1)
			return false;
	}

	return true;
}

int Triangle::ComputePointEdgeOrientation(int opposing_vertex, Vector2d pt) {
	Vector2d* v0 = NULL;
	Vector2d* v1 = NULL;
	Vector2d* v2 = &pt;

	//Setup the vertices
	if(opposing_vertex == 0) {
		v0 = GetVertex(1);
		v1 = GetVertex(2);
	}
	else if(opposing_vertex == 1) {
		v0 = GetVertex(2);
		v1 = GetVertex(0);
	}
	else if(opposing_vertex == 2) {
		v0 = GetVertex(0);
		v1 = GetVertex(1);
	}

	//Compute the orientation via a determinant
	double det = ((v0->x - v2->x)*(v1->y - v2->y) - (v0->y - v2->y)*(v1->x - v2->x));

	//Check for degeneracy
	if(fabs(det) <= EFF_ZERO)
		return 0;

	//CCW orientation
	else if(det > EFF_ZERO)
		return 1;

	//CW orientation
	else if(det < -EFF_ZERO)
		return -1;

	return 0;
}

int Triangle::OrientVertices() {
	//Get the vertices of this triangle
	Vector2d* v0 = GetVertex(0);
	Vector2d* v1 = GetVertex(1);
	Vector2d* v2 = GetVertex(2);

	//Safety check
	if(v0 == NULL || v1 == NULL || v2 == NULL)
		return false;

	//Compute the orientation for this triangle
	double det = ((v0->x - v2->x)*(v1->y - v2->y) - (v0->y - v2->y)*(v1->x - v2->x));

	//Error test for degenerate triangles
	if(fabs(det) <= EFF_ZERO)
		return false;

	//If this triangle is cw oriented
	if(det < -EFF_ZERO) {
		//Switch the first two vertices
		unsigned int v0_index = vertices[0];
		vertices[0] = vertices[1];
		vertices[1] = v0_index;
	}

	return true;
}

//Debugging functions
int Triangle::print() {
	return true;
}

int Triangle::write_svg(FILE* handle) {
	Vector2d* v0 = GetVertex(0);
	Vector2d* v1 = GetVertex(1);
	Vector2d* v2 = GetVertex(2);

	//Safety check
	if(v0 == NULL || v1 == NULL || v2 == NULL)
		return false;

	fprintf(handle, "<polygon points=\"%f,%f %f,%f %f,%f\" ", v0->x, v0->y, v1->x, v1->y, v2->x, v2->y);
	fprintf(handle, "fill=\"green\" stroke=\"black\" srtoke-width=\"2\"/>\n");

	return true;
}



TriangleEdge::TriangleEdge() {
	tindex = 0;
	opposing_vertex = 0;

	vertices[0] = 0;
	vertices[1] = 0;
}

TriangleEdge::TriangleEdge(unsigned int tindex, int opposing_vertex) {
	this->tindex = tindex;
	this->opposing_vertex = opposing_vertex;

	vertices[0] = 0;
	vertices[1] = 0;
}

TriangleEdge::~TriangleEdge() {
	//Do nothing
}

int TriangleEdge::operator<(TriangleEdge te) {
	if(tindex < te.tindex)
		return true;

	return false;
}

int TriangleEdge::operator==(TriangleEdge te) {
	if(vertices[0] == te.vertices[0] && vertices[1] == te.vertices[1])
		return true;

	if(vertices[0] == te.vertices[1] && vertices[1] == te.vertices[0])
		return true;

	return false;
}

TriangleEdge TriangleEdge::operator=(TriangleEdge te) {
	tindex = te.tindex;
	opposing_vertex = te.opposing_vertex;

	vertices[0] = te.vertices[0];
	vertices[1] = te.vertices[1];

	return (*this);
}

int TriangleEdge::GetVertices(Triangle* tri) {
	//Get the edge opposite vertex 0
	if(opposing_vertex == 0) {
		vertices[0] = tri->GetVertexIndex(1);
		vertices[1] = tri->GetVertexIndex(2);
	}

	//Get the edge opposite vertex 1
	else if(opposing_vertex == 1) {
		vertices[0] = tri->GetVertexIndex(2);
		vertices[1] = tri->GetVertexIndex(0);
	}

	//Get the edge opposite vertex 2
	else if(opposing_vertex == 2) {
		vertices[0] = tri->GetVertexIndex(0);
		vertices[1] = tri->GetVertexIndex(1);
	}

	return true;
}
