#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "triangle_mesher.h"

int main(int argc, char** argv) {
	//Seed random
	srand(time(0));

	TriangleMesher triangle_mesher;
	if(triangle_mesher.LoadCommandsFromFile(argv[1]) == false) {
		printf("Error loading file\n");
		return false;
	}

	triangle_mesher.SetFailBehaviour(TriangleMesher::QUIT_ON_FAILURE);

	triangle_mesher.PrintMesherCommands();
	triangle_mesher.RunMesherCommands();

	TriangleComplex* tc = triangle_mesher.GetTriangleComplex();

	/*Prism p;
	p.SetMin(atof(argv[2]), atof(argv[3]));
	p.SetMax(atof(argv[4]), atof(argv[5]));

	unsigned int edge_count;
	double avg_edge_length;
	tc->ComputeEdgeStatisticsInsidePrism(edge_count, avg_edge_length, p);
	printf("edge count: %u\n", edge_count);
	printf("average edge length: %f\n", avg_edge_length);*/

	return 0;
}
