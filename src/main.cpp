#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "triangle_mesher.h"

int main(int argc, char** argv) {
	//Seed random
	srand(time(0));

	char filename[1000];
	if(argc >= 2)
		sprintf(filename, "%s", argv[1]);

	else {
		printf("Mesher command file: ");
		int ret = scanf("%s", filename);
	}

	TriangleMesher triangle_mesher;
	if(triangle_mesher.LoadCommandsFromFile(filename) == false) {
		printf("Error loading file %s\n", filename);
		return false;
	}

	triangle_mesher.SetFailBehaviour(TriangleMesher::QUIT_ON_FAILURE);

	triangle_mesher.PrintMesherCommands();
	triangle_mesher.RunMesherCommands();

	TriangleComplex* tc = triangle_mesher.GetTriangleComplex();

	return 0;
}
