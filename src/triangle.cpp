#include "triangle.h"

int TestLineSegmentOverlap(Vector2d v1, Vector2d v2, Vector2d w1, Vector2d w2) {
	//Special case: the w-line is vertical
	if(fabs(w1.x - w2.x) < EFF_ZERO) {
		//Special case: the v-line is also vertical
		if(fabs(v1.x - v2.x) < EFF_ZERO) {
			//Check for degeneracy
			if(fabs(w1.y - w2.y) < EFF_ZERO)
				return false;

			double t = (v1.y - w1.y) / (w2.y - w1.y);

			//There was an overlap
			if(t >= 0 && t <= 1)
				return true;

			t = (v2.y - w1.y) / (w2.y - w1.y);

			//There was an overlap
			if(t >= 0 && t <= 1)
				return true;
		}

		//General case
		else {
			double t = (w1.x - v1.x) / (v2.x - v1.x);

			//There was an overlap
			if(t >= 0 && t <= 1)
				return true;
		}
	}

	//Otherwise use a general procedure
	else {
		double w_slope = (w2.y - w1.y) / (w2.x - w1.x);

		//Solve for the parameter describing the v-line
		double t = (w1.y - v1.y) + w_slope*(v1.x - w1.x);
		t /= (v2.y - v1.y) + w_slope*(v2.x - v1.x);

		//There was an overlap
		if(t >= 0 && t <= 1)
			return true;
	}

	//There was no overlap
	return false;
}

//Initialize the global triangle count
unsigned int Triangle::global_triangle_count = 0;

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

	//Establish the local index
	global_triangle_count++;
	local_index = global_triangle_count;
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

Vector2d* Triangle::GetGlobalVertex(unsigned int vindex) {
	//Safety check
	if(vindex >= global_vertex_list->size())
		return NULL;

	return (*global_vertex_list)[vindex];
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

int Triangle::IsVertex(unsigned int vindex) {
	if(vertices[0] == vindex) return true;
	if(vertices[1] == vindex) return true;
	if(vertices[2] == vindex) return true;

	return false;
}

int Triangle::GetAdjacentTriangleCount() {
	int count = 0;

	for(int i=0; i<3; i++)
		if(adjacent_triangles[i] != NULL)
			count++;

	return count;
}

int Triangle::SetAdjacentTriangle(int opposing_vertex, Triangle* tri) {
	adjacent_triangles[opposing_vertex] = tri;

	return true;
}

Triangle* Triangle::GetAdjacentTriangle(int opposing_vertex) {
	return adjacent_triangles[opposing_vertex];
}

int Triangle::TestAdjacency(Triangle* tri, int& opposing_vertex, int& tri_opposing_vertex) {
	//See if tri has two vertices in common with this one
	int vertices_in_common = 0;

	opposing_vertex = -1;
	tri_opposing_vertex = -1;

	for(int i=0; i<3; i++) {
		int found_vertex = false;

		for(int j=0; j<3; j++) {
			if(GetVertex(i) == tri->GetVertex(j)) {
				found_vertex = true;
				vertices_in_common++;

				break;
			}
		}

		if(found_vertex == false)
			opposing_vertex = i;
	}

	for(int i=0; i<3; i++) {
		int found_vertex = false;

		for(int j=0; j<3; j++) {
			if(tri->GetVertex(i) == GetVertex(j)) {
				found_vertex = true;
				break;
			}
		}

		if(found_vertex == false) {
			tri_opposing_vertex = i;
			break;
		}
	}

	if(vertices_in_common == 2)
		return true;

	opposing_vertex = -1;
	tri_opposing_vertex = -1;
	return false;
}

unsigned int Triangle::GetLocalIndex() {
	return local_index;
}

int Triangle::SetLocalIndex(unsigned int local_index) {
	this->local_index = local_index;

	if(local_index >= global_triangle_count)
		global_triangle_count = local_index;

	return true;
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

//Test Delaunay condition with an adjacent triangle
int Triangle::TestDelaunay(int opposing_vertex) {
	//Safety test, if the adjacent triangle doesn't exist then the condition is satisfied
	Triangle* adj_tri = GetAdjacentTriangle(opposing_vertex);
	if(adj_tri == NULL)
		return true;

	//Figure out which of the three vertices of the other triangle is not in this one
	unsigned int external_vindex = 0;

	if(IsVertex(adj_tri->GetVertexIndex(0)) == false)
		external_vindex = adj_tri->GetVertexIndex(0);

	else if(IsVertex(adj_tri->GetVertexIndex(1)) == false)
		external_vindex = adj_tri->GetVertexIndex(1);

	else if(IsVertex(adj_tri->GetVertexIndex(2)) == false)
		external_vindex = adj_tri->GetVertexIndex(2);

	//These triangles are somehow identical, which is an error, but return true anyway
	else
		return true;

	Vector2d* ve = GetGlobalVertex(external_vindex);
	if(ve == NULL)
		return true;

	//Test the Delaunay condition
	if(TestPointInsideCircumcircle(*ve) == true)
		return false;


	//Figure out which of the three vertices of this triangle is not in the other one
	unsigned int internal_vindex = 0;

	if(adj_tri->IsVertex(GetVertexIndex(0)) == false)
		internal_vindex = GetVertexIndex(0);

	else if(adj_tri->IsVertex(GetVertexIndex(1)) == false)
		internal_vindex = GetVertexIndex(1);

	else if(adj_tri->IsVertex(GetVertexIndex(2)) == false)
		internal_vindex = GetVertexIndex(2);

	//These triangles are somehow identical, which is an error, but return true anyway
	else
		return true;

	Vector2d* vi = GetGlobalVertex(internal_vindex);
	if(vi == NULL)
		return true;

	//Test the Delaunay condition
	if(adj_tri->TestPointInsideCircumcircle(*vi) == true)
		return false;

	//Lastly check to make sure the edge actually lies inbetween the two vertices
	Vector2d* v0 = NULL;
	Vector2d* v1 = NULL;

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

	//Safety check
	if(v0 == NULL || v1 == NULL)
		return true;

	//if(TestLineSegmentOverlap(*vi, *ve, *v0, *v1) == false)
	//	return true;

	//Neither of the two Delaunay tests failed
	return true;
}

//Perform a Delaunay flip with an adjacent triangle
int Triangle::PerformDelaunayFlip(int opposing_vertex) {
	//Safety test, don't do anything if there is no adjacent triangle
	Triangle* adj_tri = GetAdjacentTriangle(opposing_vertex);
	if(adj_tri == NULL)
		return false;

	unsigned int combined_vertices[4];
	Triangle* adjacent_triangles[4];

	combined_vertices[0] = 0;
	combined_vertices[1] = 0;
	combined_vertices[2] = 0;
	combined_vertices[3] = 0;

	adjacent_triangles[0] = NULL;
	adjacent_triangles[1] = NULL;
	adjacent_triangles[2] = NULL;
	adjacent_triangles[3] = NULL;

	//printf("ASDF\n");
	for(int i=0; i<3; i++) {
		if(adj_tri->IsVertex(GetVertexIndex(i)) == false)
			combined_vertices[0] = GetVertexIndex(i);

		if(IsVertex(adj_tri->GetVertexIndex(i)) == false)
			combined_vertices[2] = adj_tri->GetVertexIndex(i);
	}
	//combined_vertices[0] = GetVertexIndex(opposing_vertex);

	//printf("LALA\n");
	combined_vertices[1] = GetNextVertex(combined_vertices[0]);
	combined_vertices[3] = GetPrevVertex(combined_vertices[0]);

	/*combined_vertices[3] = adj_tri->GetNextVertex(combined_vertices[2]);
	if(combined_vertices[3] == combined_vertices[1])
		combined_vertices[3] = adj_tri->GetPrevVertex(combined_vertices[2]);*/

	//printf("%u %u %u %u %u %u\n", GetVertexIndex(0), GetVertexIndex(1), GetVertexIndex(2), adj_tri->GetVertexIndex(0), adj_tri->GetVertexIndex(1), adj_tri->GetVertexIndex(2));
	//printf("%u %u %u %u\n", combined_vertices[0], combined_vertices[1], combined_vertices[2], combined_vertices[3]);

	for(int i=0; i<3; i++) {
		if(GetVertexIndex(i) == combined_vertices[3])
			adjacent_triangles[0] = GetAdjacentTriangle(i);

		if(GetVertexIndex(i) == combined_vertices[1])
			adjacent_triangles[3] = GetAdjacentTriangle(i);

		if(adj_tri->GetVertexIndex(i) == combined_vertices[3])
			adjacent_triangles[1] = adj_tri->GetAdjacentTriangle(i);

		if(adj_tri->GetVertexIndex(i) == combined_vertices[1])
			adjacent_triangles[2] = adj_tri->GetAdjacentTriangle(i);
	}

	//Update the adjacent triangles of the adjacent triangles 1 and 3
	for(int i=0; i<3; i++) {
		if(adjacent_triangles[1] && adj_tri->IsVertex(adjacent_triangles[1]->GetVertexIndex(i)) == false)
			adjacent_triangles[1]->SetAdjacentTriangle(i, this);

		if(adjacent_triangles[3] && IsVertex(adjacent_triangles[3]->GetVertexIndex(i)) == false)
			adjacent_triangles[3]->SetAdjacentTriangle(i, adj_tri);
	}

	//printf("OKKOK\n");
	//Flip the vertices around
	SetVertex(0, combined_vertices[1]);
	SetVertex(1, combined_vertices[0]);
	SetVertex(2, combined_vertices[2]);

	adj_tri->SetVertex(0, combined_vertices[3]);
	adj_tri->SetVertex(1, combined_vertices[2]);
	adj_tri->SetVertex(2, combined_vertices[0]);

	//printf("BNLA\n");
	//Flip the adjacent triangles around
	SetAdjacentTriangle(0, adj_tri);
	SetAdjacentTriangle(1, adjacent_triangles[1]);
	SetAdjacentTriangle(2, adjacent_triangles[0]);

	adj_tri->SetAdjacentTriangle(0, this);
	adj_tri->SetAdjacentTriangle(1, adjacent_triangles[3]);
	adj_tri->SetAdjacentTriangle(2, adjacent_triangles[2]);

	//printf("OPKOK\n");
	OrientVertices();
	adj_tri->OrientVertices();
	//printf("ALL GOOD\n");

	return true;
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

int Triangle::GetCentroid(Vector2d& centroid) {
	centroid.x = 0.0;
	centroid.y = 0.0;

	int count = 0;
	for(int i=0; i<3; i++) {
		Vector2d* pt = GetVertex(i);

		if(pt != NULL) {
			centroid.x += pt->x;
			centroid.y += pt->y;

			count++;
		}
	}

	if(count == 0)
		return false;

	centroid /= double(count);
	return true;
}

int Triangle::GetVertexAngle(int vertex, double& angle) {
	Vector2d* v0 = GetVertex(vertex);
	Vector2d* v1 = NULL;
	Vector2d* v2 = NULL;

	if(vertex == 0) {
		v1 = GetVertex(1);
		v2 = GetVertex(2);
	}
	else if(vertex == 1) {
		v1 = GetVertex(2);
		v2 = GetVertex(0);
	}
	else if(vertex == 2) {
		v1 = GetVertex(0);
		v2 = GetVertex(1);
	}

	angle = 0.0;
	if(v0 == NULL || v1 == NULL || v2 == NULL)
		return false;

	Vector2d edge1 = ((*v1) - (*v0));
	Vector2d edge2 = ((*v2) - (*v0));

	angle = edge1.angle(edge2);
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
		SetVertex(0, GetVertexIndex(1));
		SetVertex(1, v0_index);
		//vertices[0] = vertices[1];
		//vertices[1] = v0_index;

		//Also switch the adjacent triangles across from those vertices
		Triangle* adj_tri = GetAdjacentTriangle(0);
		SetAdjacentTriangle(0, GetAdjacentTriangle(1));
		SetAdjacentTriangle(1, adj_tri);
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

	//Draw the centroid
	/*Vector2d centroid;

	if(GetCentroid(centroid) == true) {
		double cx = centroid.x;
		double cy = centroid.y;
		double r = 3.0;
		fprintf(handle, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"purple\"/>\n", cx, h-cy, r);

		//Draw the connections to adjacent triangles
		for(int i=0; i<3; i++) {
			Triangle* adj_tri = GetAdjacentTriangle(i);

			if(adj_tri != NULL) {
				Vector2d adj_centroid;

				if(adj_tri->GetCentroid(adj_centroid) == true) {
					fprintf(handle, "<line x1=\"%f\" y1=\"%f\" ", centroid.x, h-centroid.y);
					fprintf(handle, "x2=\"%f\" y2=\"%f\" stroke-width=\"2\" stroke=\"red\"/>\n", adj_centroid.x, h-adj_centroid.y);
				}
			}
		}
	}*/

	/*if(compute_circumcircle() == true) {
		//Draw the circumcenter
		double cx = circumcenter->x;
		double cy = circumcenter->y;
		double r = 10.0;
		fprintf(handle, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"purple\"/>\n", cx, h-cy, r);

		for(int i=0; i<3; i++) {
			Triangle* adj_tri = GetAdjacentTriangle(i);

			if(adj_tri != NULL) {
				Vector2d adj_circumcenter;
				double adj_circumradius = 0.0;

				if(adj_tri->GetCircumcircle(adj_circumcenter, adj_circumradius) == true) {
					fprintf(handle, "<line x1=\"%f\" y1=\"%f\" ", circumcenter->x, h-circumcenter->y);
					fprintf(handle, "x2=\"%f\" y2=\"%f\" stroke-width=\"2\" stroke=\"red\"/>\n", adj_circumcenter.x, h-adj_circumcenter.y);
				}
			}
		}
	}*/

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
	tri = NULL;
	opposing_vertex = 0;

	vertices[0] = 0;
	vertices[1] = 0;
}

TriangleEdge::TriangleEdge(Triangle* tri, int opposing_vertex) {
	if(tri == NULL) {
		this->tri = NULL;
		this->opposing_vertex = 0;
	}

	else {
		this->tri = tri;
		this->opposing_vertex = opposing_vertex;

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
	}
}

TriangleEdge::~TriangleEdge() {
	//Do nothing
}

int TriangleEdge::operator==(TriangleEdge te) {
	if(vertices[0] == te.vertices[0] && vertices[1] == te.vertices[1])
		return true;

	if(vertices[0] == te.vertices[1] && vertices[1] == te.vertices[0])
		return true;

	return false;
}

TriangleEdge TriangleEdge::operator=(TriangleEdge te) {
	tri = te.tri;
	opposing_vertex = te.opposing_vertex;

	vertices[0] = te.vertices[0];
	vertices[1] = te.vertices[1];

	return (*this);
}
