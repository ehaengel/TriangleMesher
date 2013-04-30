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

	//File i/o function
	int LoadFromTagData(XML_Tag* mesh_command_tag);

	//Debugging function
	int print();


	/////////////////////////////
	// The Mesher Command Type //
	/////////////////////////////

	int command_type;
	enum {
		DO_NOTHING=0,

		GENERATE_RANDOM_GRID,
		GENERATE_UNIFORM_GRID,
		GENERATE_HEX_GRID,

		RUN_TRIANGLE_MESHER,

		LOAD_MESH_FROM_FILE,
		SAVE_MESH_TO_FILE,

		WRITE_SVG,

		APPEND_VERTEX,

		SUBDIVIDE_TRIANGLE,
		BARYCENTRIC_SUBDIVIDE,

		BASIC_TRIANGLE_MESHER,
		BASIC_DELAUNAY_FLIPPER,

		STRETCHED_GRID,

		REFINE_MESH
	};

	////////////////////////////////
	// Mesher Command option data //
	////////////////////////////////

	//Generate Random/Uniform/Hex Grid options
	double xmin, xmax;
	double ymin, ymax;

	unsigned int vertex_count;
	unsigned int xcount;
	unsigned int ycount;

	//Run Triangle Mesher options
	int use_kd_tree;

	//Load/Save Mesh From/To File options
	char filename[1000];
	int load_save_triangles;

	//Write SVG options
	char svg_filename[1000];
	double svg_width;
	double svg_height;

	//Append Vertex options
	Vector2d vertex;

	//Subdivide triangle/Barycentric subdivision options
	unsigned int subdivide_vindex;
	unsigned int subdivide_triangle_local_index;

	//Basic Triangle Mesher options

	//Basic Delaunay Flipper options
	unsigned int delaunay_max_iterations;

	//Stretched Grid options
	unsigned int stretched_grid_iterations;
	double stretched_grid_alpha;

	//Refine mesh options
	double desired_edge_length;

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
