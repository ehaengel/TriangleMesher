#include "mesher_command.h"

MesherCommand::MesherCommand() {
	/////////////////////////////
	// The Mesher Command Type //
	/////////////////////////////

	command_type = MesherCommand::DO_NOTHING;

	////////////////////////////////
	// Mesher Command option data //
	////////////////////////////////

	//Generate Random/Uniform Grid options
	xmin = 0.0;
	xmax = 0.0;
	ymin = 0.0;
	ymax = 0.0;

	vertex_count = 0;
	xcount = 0;
	ycount = 0;

	//Run Triangle Mesher options
	use_kd_tree = true;

	//Load/Save Mesh From/To File options
	filename = "";

	//Append Vertex options
	vertex = Vector2d(0, 0);

	//Basic Triangle Mesher options

	//Basic Delaunay Flipper options
	delaunay_max_iterations = 100;

	//Stretched Grid options

	/////////////////////////////////
	// Mesher Command Results data //
	/////////////////////////////////

	mesher_command_result = MesherCommand::NOT_YET_RUN_RESULT;
}

MesherCommand::~MesherCommand() {
	//Do nothing
}

