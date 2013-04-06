#include "mesher_command.h"

MesherCommand::MesherCommand() {
	//The Mesher Command Type
	command_type = MesherCommand::DO_NOTHING;

	//Generate Random/Uniform Grid options
	xmin = 0.0;
	xmax = 0.0;
	ymin = 0.0;
	ymax = 0.0;

	random_vertex_count = 0;
	x_vertex_count = 0;
	y_vertex_count = 0;

	//Triangle Mesher options
	use_kd_tree = true;

	//Basic Triangle Mesher options

	//Basic Delaunay Flipper options

	//Stretched Grid options
}

MesherCommand::~MesherCommand() {
	//Do nothing
}

