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

	/////////////////////////////
	// The Mesher Command Type //
	/////////////////////////////

	int command_type;
	enum {
		DO_NOTHING=0,

		GENERATE_RANDOM_GRID,
		GENERATE_UNIFORM_GRID,

		RUN_TRIANGLE_MESHER,

		LOAD_MESH_FROM_FILE,
		SAVE_MESH_TO_FILE,

		APPEND_VERTEX,

		BASIC_TRIANGLE_MESHER,
		BASIC_DELAUNAY_FLIPPER,

		STRETCHED_GRID
	};

	////////////////////////////////
	// Mesher Command option data //
	////////////////////////////////

	//Generate Random/Uniform Grid options
	double xmin, xmax;
	double ymin, ymax;

	unsigned int vertex_count;
	unsigned int xcount;
	unsigned int ycount;

	//Run Triangle Mesher options
	int use_kd_tree;

	//Load/Save Mesh From/To File options
	const char* filename;

	//Append Vertex options
	Vector2d vertex;

	//Basic Triangle Mesher options

	//Basic Delaunay Flipper options
	unsigned int delaunay_max_iterations;

	//Stretched Grid options

	/////////////////////////////////
	// Mesher Command Results data //
	/////////////////////////////////

	int mesher_command_result;
	enum {
		NOT_YET_RUN_RESULT=0,
		FAILURE_RESULT,
		SUCCESS_RESULT
	};
};

#endif
