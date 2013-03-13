#include "triangle.h"

Triangle::Triangle(VertexList* global_vertex_list) {
	this->global_vertex_list = global_vertex_list;

	vertices[0] = 0;
	vertices[1] = 0;
	vertices[2] = 0;

	adjacent_triangles[0] = NULL;
	adjacent_triangles[1] = NULL;
	adjacent_triangles[2] = NULL;

	circumcenter = NULL;
	circumradius = 0.0;
}

Triangle::~Triangle() {
	if(circumcenter)
		delete circumcenter;
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

	//If the vertices have changed reset the circumcircle
	if(circumcenter != NULL) {
		delete circumcenter;
		circumcenter = NULL;

		circumradius = 0.0;
	}

	return true;
}

int Triangle::SetVertex(int vertex, unsigned int vindex) {
	//Safety check
	if(vertex < 0 || vertex > 2)
		return false;

	//If the vertices have changed reset the circumcircle
	if(circumcenter != NULL) {
		delete circumcenter;
		circumcenter = NULL;

		circumradius = 0.0;
	}

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

int Triangle::SetAdjacentTriangle(int vertex, Triangle* tri) {
	adjacent_triangles[vertex] = tri;

	return true;
}

Triangle* Triangle::GetAdjacentTriangle(int vertex) {
	return adjacent_triangles[vertex];
}

//Geometric primitives
int Triangle::TestPointInside(Vector2d pt, bool soft_edges) {
	//First check if the point is inside the circumcircle for a quick test
	if(TestPointInsideCircumcircle(pt) == false)
		return false;

	//Check if the point is inside the open triangle
	if(soft_edges) {
		if(TestPointEdgeOrientation(0, pt) <= 0)
			return false;

		else if(TestPointEdgeOrientation(1, pt) <= 0)
			return false;

		else if(TestPointEdgeOrientation(2, pt) <= 0)
			return false;
	}

	//Check if the point is inside the closed triangle
	else {
		if(TestPointEdgeOrientation(0, pt) == -1)
			return false;

		else if(TestPointEdgeOrientation(1, pt) == -1)
			return false;

		else if(TestPointEdgeOrientation(2, pt) == -1)
			return false;
	}

	return true;
}

//Tests if a point is inside the circumcircle
int Triangle::TestPointInsideCircumcircle(Vector2d pt) {
	//Make sure our circumcircle is computed, and if we are degenerate then we overlap everything
	if(compute_circumcircle() == false)
		return true;

	if(circumcenter->distance2(pt) <= circumradius*circumradius)
		return true;

	return false;
}

int Triangle::TestPointEdgeOrientation(int opposing_vertex, Vector2d pt) {
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

	//Safety check for degeneracy, degenerate triangle edges are collinear with all points
	if(v0 == NULL || v1 == NULL || v2 == NULL)
		return 0;

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

//Tests if the triangle circumcircles overlap
int Triangle::TestCircumcircleOverlap(Triangle* tri) {
	//Make sure our circumcircle is computed, and if we are degenerate then we overlap everything
	if(compute_circumcircle() == false)
		return true;

	//Try to get tri's circumcircle, and if tri is degenerate then it overlaps everything
	Vector2d tri_circumcenter;
	double tri_circumradius = 0.0;

	if(tri->GetCircumcircle(tri_circumcenter, tri_circumradius) == false)
		return true;

	//Perform the test
	double rsum = circumradius + tri_circumradius;
	double dist2 = circumcenter->distance2(tri_circumcenter);

	if(dist2 < rsum*rsum)
		return true;

	return false;
}

//Tests overlap using splitting planes
int Triangle::TestOverlapSplittingPlanes(Triangle* tri) {
	//See if any of the six edges forms a splitting plane

	//Test the three edges of tri
	Vector2d* v0 = GetVertex(0);
	Vector2d* v1 = GetVertex(1);
	Vector2d* v2 = GetVertex(2);

	//Safety check, degenerate triangles overlap with everything
	if(v0 == NULL || v1 == NULL || v2 == NULL)
		return true;

	int found_splitting_plane = false;
	for(int i=0; i<3; i++) {
		//Test if all three points v0, v1, v2 form a cw triangle with this edge
		if(tri->TestPointEdgeOrientation(i, *v0) == 1)
			continue;

		if(tri->TestPointEdgeOrientation(i, *v1) == 1)
			continue;

		if(tri->TestPointEdgeOrientation(i, *v2) == 1)
			continue;

		found_splitting_plane = true;
		break;
	}

	//We conclude there is no overlap
	if(found_splitting_plane)
		return false;

	//Test the three edges of this triangle
	Vector2d* tri_v0 = tri->GetVertex(0);
	Vector2d* tri_v1 = tri->GetVertex(1);
	Vector2d* tri_v2 = tri->GetVertex(2);

	//Safety check, degenerate triangles overlap with everything
	if(tri_v0 == NULL || tri_v1 == NULL || tri_v2 == NULL)
		return true;

	found_splitting_plane = false;
	for(int i=0; i<3; i++) {
		//Test if all three points tri_v0, tri_v1, tri_v2 form a cw triangle with this edge
		if(TestPointEdgeOrientation(i, *tri_v0) == 1)
			continue;

		if(TestPointEdgeOrientation(i, *tri_v1) == 1)
			continue;

		if(TestPointEdgeOrientation(i, *tri_v2) == 1)
			continue;

		found_splitting_plane = true;
		break;
	}

	//We conclude there is no overlap
	if(found_splitting_plane)
		return false;

	//We could not find a splitting plane so there must be an overlap
	return true;
}

//General test for triangle overlap
int Triangle::TestOverlap(Triangle* tri) {
	//First go for the circumcircle test, because it is generally fast
	if(TestCircumcircleOverlap(tri) == false)
		return false;

	//The circumcircles do overlap, so do the splitting plane test
	return TestOverlapSplittingPlanes(tri);
}

int Triangle::GetCircumcircle(Vector2d& center, double& radius) {
	//For safety, set some default values
	center.x = 0.0;
	center.y = 0.0;
	radius = 0.0;

	//If the circumcircle has yet to be calculated then calculate it
	if(compute_circumcircle() == false)
		return false;

	center = *circumcenter;
	radius = circumradius;

	return true;
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

int Triangle::write_svg(FILE* handle, double w, double h) {
	Vector2d* v0 = GetVertex(0);
	Vector2d* v1 = GetVertex(1);
	Vector2d* v2 = GetVertex(2);

	//Safety check
	if(v0 == NULL || v1 == NULL || v2 == NULL)
		return false;

	fprintf(handle, "<polygon points=\"%f,%f %f,%f %f,%f\" ", v0->x, h-v0->y, v1->x, h-v1->y, v2->x, h-v2->y);
	fprintf(handle, "fill=\"green\" stroke=\"black\" srtoke-width=\"2\" style=\"fill-opacity:0.5\"/>\n");

	return true;
}

//Internal use functions
int Triangle::compute_circumcircle() {
	if(circumcenter == NULL) {
		Vector2d* v0 = GetVertex(0);
		Vector2d* v1 = GetVertex(1);
		Vector2d* v2 = GetVertex(2);

		//Safety check
		if(v0 == NULL || v1 == NULL || v2 == NULL)
			return false;

		double D = 2*(v0->x*(v1->y - v2->y) + v1->x*(v2->y - v0->y) + v2->x * (v0->y - v1->y));

		//Safety check for a degenerate triangle
		if(fabs(D) < EFF_ZERO)
			return false;

		double Ux = v0->mag2()*(v1->y - v2->y) + v1->mag2()*(v2->y - v0->y) + v2->mag2()*(v0->y - v1->y);
		double Uy = v0->mag2()*(v2->x - v1->x) + v1->mag2()*(v0->x - v2->x) + v2->mag2()*(v1->x - v0->x);

		circumcenter = new Vector2d(Ux/D, Uy/D);
		circumradius = circumcenter->distance(*v2);
	}

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

	//Return false if one of the vertices is null
	if(vertices[0] == 0 || vertices[1] == 0)
		return false;

	//Otherwise return true
	return true;
}
