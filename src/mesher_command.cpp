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
	strcpy(filename, "");

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

//File i/o function
int MesherCommand::LoadFromTagData(XML_Tag* mesh_command_tag) {
	string command_type_str = mesh_command_tag->GetTagName();

	if(strcmp(command_type_str.c_str(), "GenerateRandomGrid") == 0) {
		command_type = MesherCommand::GENERATE_RANDOM_GRID;

		string xmin_str, xmax_str;
		string ymin_str, ymax_str;
		string vertex_count_str;

		xmin_str = mesh_command_tag->GetAttributeValue("xmin");
		xmax_str = mesh_command_tag->GetAttributeValue("xmax");
		ymin_str = mesh_command_tag->GetAttributeValue("ymin");
		ymax_str = mesh_command_tag->GetAttributeValue("ymax");
		vertex_count_str = mesh_command_tag->GetAttributeValue("vertex_count");

		if(xmin_str != "") xmin = atof(xmin_str.c_str());
		if(xmax_str != "") xmax = atof(xmax_str.c_str());
		if(ymin_str != "") ymin = atof(ymin_str.c_str());
		if(ymax_str != "") ymax = atof(ymax_str.c_str());
		if(vertex_count_str != "") vertex_count = (unsigned int) atoi(vertex_count_str.c_str());
	}

	else if(strcmp(command_type_str.c_str(), "TriangleMesher") == 0) {
		command_type = MesherCommand::RUN_TRIANGLE_MESHER;

		string use_kd_tree_str = mesh_command_tag->GetAttributeValue("use_kd_tree");
		if(strcmp(use_kd_tree_str.c_str(), "yes") == 0)
			use_kd_tree = true;

		else if(strcmp(use_kd_tree_str.c_str(), "no") == 0)
			use_kd_tree = false;
	}

	else if(strcmp(command_type_str.c_str(), "LoadMeshFromFile") == 0) {
		command_type = MesherCommand::LOAD_MESH_FROM_FILE;

		string filename_str = mesh_command_tag->GetAttributeValue("filename");

		if(filename_str != "")
			strncpy(filename, filename_str.c_str(), 1000);
	}

	else if(strcmp(command_type_str.c_str(), "SaveMeshToFile") == 0) {
		command_type = MesherCommand::SAVE_MESH_TO_FILE;

		string filename_str = mesh_command_tag->GetAttributeValue("filename");

		if(filename_str != "")
			strncpy(filename, filename_str.c_str(), 1000);
	}

	else
		command_type = MesherCommand::DO_NOTHING;

	return true;
}

//Debugging function
int MesherCommand::print() {
	if(command_type == MesherCommand::DO_NOTHING)
		printf("Mesher command: Do nothing\n");

	else if(command_type == MesherCommand::GENERATE_RANDOM_GRID) {
		printf("Mesher command: Generate random grid\n");
		printf("xmin: %f; xmax: %f\n", xmin, xmax);
		printf("ymin: %f; ymax: %f\n", ymin, ymax);
		printf("random vertex count: %u\n", vertex_count);
	}

	else if(command_type == MesherCommand::GENERATE_UNIFORM_GRID) {
		printf("Mesher command: Generate uniform grid\n");
		printf("xmin: %f; xmax: %f\n", xmin, xmax);
		printf("ymin: %f; ymax: %f\n", ymin, ymax);
		printf("x vertex count: %u\n", xcount);
		printf("y vertex count: %u\n", ycount);
	}

	else if(command_type == MesherCommand::RUN_TRIANGLE_MESHER) {
		printf("Mesher command: Run triangle mesher\n");
		printf("use_kd_tree: %d\n", use_kd_tree);
	}

	else if(command_type == MesherCommand::LOAD_MESH_FROM_FILE) {
		printf("Mesher command: Load mesh from file\n");
		printf("filename: %s\n", filename);
	}

	else if(command_type == MesherCommand::SAVE_MESH_TO_FILE) {
		printf("Mesher command: Save mesh to file\n");
		printf("filename: %s\n", filename);
	}

	else {
		printf("Mesher Command: Error, undefined command\n");
		return false;
	}

	return true;
}
