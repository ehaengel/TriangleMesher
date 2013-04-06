#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

//XML i/o code
#include "SimpleXML/src/xml_document.h"

//Triangulation algorithm related code
#include "utility.h"
#include "vector2d.h"

#ifndef MESHER_COMMAND
#define MESHER_COMMAND

class MesherCommand {
public:
	MesherCommand();
	~MesherCommand();

	//These are the available mesher commands
	int command_type;
	enum {
		DO_NOTHING=0,
		GENERATE_RANDOM_GRID,
		GENERATE_UNIFORM_GRID,
		TRIANGLE_MESHER,
		BASIC_TRIANGLE_MESHER,
		BASIC_DELAUNAY_FLIPPER,
		STRETCHED_GRID
	};

	//Generate Random/Uniform Grid options
	double xmin, xmax;
	double ymin, ymax;

	unsigned int random_vertex_count;
	unsigned int x_vertex_count;
	unsigned int y_vertex_count;

	//Triangle Mesher options
	int use_kd_tree;

	//Basic Triangle Mesher options

	//Basic Delaunay Flipper options

	//Stretched Grid options
};

#endif
