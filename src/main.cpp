#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "triangle_complex.h"

int main(int argc, char** argv) {
	//Seed random
	srand(time(0));

	TriangleComplex* triangle_complex = new TriangleComplex;

	if(argc < 3) {
		printf("Run random simulation (y/n): ");

		char is_random = getchar();
		if(is_random == 'y') {
			printf("How many vertices would you like to use: ");

			int vertex_count = 0;
			scanf("%d", &vertex_count);

			triangle_complex->generate_random_vertex_list(vertex_count, 0.0, 1000.0);

			if(triangle_complex->RunTriangleMesher() == false)
				printf("Error occured in the mesher\n");

			else {
				triangle_complex->write_svg("out_final.svg", 1000.0, 1000.0);
				triangle_complex->WriteToFile("test.msh");
			}
		}

		else if(is_random == 'n') {
			printf("What is the file you wish to load from: ");

			char buffer[1000];
			scanf("%s", buffer);

			triangle_complex->LoadFromFile(buffer);

			if(triangle_complex->RunTriangleMesher() == false)
				printf("Error occured in the mesher\n");

			else {
				triangle_complex->write_svg("out_final.svg", 1000.0, 1000.0);
				triangle_complex->WriteToFile("test.msh");
			}
		}

		else {
			printf("Invalid input\n\n");
			delete triangle_complex;
		}

		printf("TYPE SOMETHING AND HIT ENTER TO CONTINUE...\n");
		char buffer[1000];
		scanf("%s", buffer);
	}
	else {
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

		triangle_complex->write_svg("out_final.svg", 1000.0, 1000.0);
		triangle_complex->WriteToFile("test.msh");
	}

	return 0;
}
