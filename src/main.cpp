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

	vector<Edge*> edge_list;
	tc->GetEdges(edge_list);
	printf("edge count: %u\n", edge_list.size());

	for(unsigned int i=0; i<edge_list.size(); i++)
		printf("%u %u\n", edge_list[i]->GetVertexIndex(0), edge_list[i]->GetVertexIndex(1));

	return 0;
}
