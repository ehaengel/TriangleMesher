#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "triangle_complex.h"

int main(int argc, char** argv) {
	/*Vector2d v0(1.0, 1.0);
	Vector2d v1(2.0, 2.0);
	Vector2d v2(3.0, 1.0);

	Vector2d pt(atof(argv[1]), atof(argv[2]));

	VertexList global_vertex_list;
	global_vertex_list.push_back(NULL);
	global_vertex_list.push_back(&v0);
	global_vertex_list.push_back(&v1);
	global_vertex_list.push_back(&v2);

	Triangle tri(&global_vertex_list);
	tri.SetVertex(1);
	tri.SetVertex(2);
	tri.SetVertex(3);
	if(tri.OrientVertices() == false)
		printf("Error\n");

	printf("%u %u %u\n", tri.GetVertexIndex(0), tri.GetVertexIndex(1), tri.GetVertexIndex(2));

	printf("test v0: %d\n", tri.TestPointEdgeOrientation(0, pt));
	printf("test v1: %d\n", tri.TestPointEdgeOrientation(1, pt));
	printf("test v2: %d\n", tri.TestPointEdgeOrientation(2, pt));*/

	//Seed random
	srand(time(0));

	TriangleComplex* triangle_complex = new TriangleComplex;

	if(argc > 2 && strcmp(argv[1], "-m") == 0)
		triangle_complex->LoadFromFile(argv[2]);

	else if(argc > 2 && strcmp(argv[1], "-r") == 0)
		triangle_complex->generate_random_vertex_list(atoi(argv[2]), 0.0, 1000.0);

	else {
		printf("Please supply an argument\n");

		delete triangle_complex;
		return 1;
	}

	if(triangle_complex->RunTriangleMesher() == false) {
		printf("Error occured in the mesher\n");
		//return 1;
	}
	//triangle_complex->write_svg("out.svg", 1000.0, 1000.0);

	/*if(triangle_complex->RunDelaunayFlips() == false) {
		printf("Error occured in the delaunay flipper\n");
		//return 1;
	}*/

	triangle_complex->write_svg("out2.svg", 1000.0, 1000.0);
	triangle_complex->WriteToFile("test.msh");

	return 0;
}
