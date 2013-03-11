#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "triangle_complex.h"

int main(int argc, char** argv) {
	//Seed random
	srand(time(0));

	TriangleComplex* triangle_complex = new TriangleComplex;

	triangle_complex->generate_random_vertex_list(atoi(argv[1]), 0.0, 300.0);

	if(triangle_complex->RunTriangleMesher() == false) {
		printf("Error occured\n");
		//return 1;
	}

	triangle_complex->write_svg("out.svg", 300.0, 300.0);
	triangle_complex->WriteToFile("test.msh");

	return 0;
}
