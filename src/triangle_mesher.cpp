#include "triangle_mesher.h"

TriangleMesher::TriangleMesher() {
	//Data regarding the mesher command stack
	mesher_command_stack.clear();

	mesher_command_fail_behaviour = TriangleMesher::QUIT_ON_FAILURE;

	//The mesh and a complex for the mesher to operate on
	global_mesh_data = new GlobalMeshData;
	triangle_complex = new TriangleComplex(global_mesh_data);
}

TriangleMesher::~TriangleMesher() {
	delete triangle_complex;
	triangle_complex = NULL;
}

////////////////////////////////////////
// Triangle Mesher Command Management //
////////////////////////////////////////

//These functions are used to append/remove commands to run later
int TriangleMesher::LoadCommandsFromFile(const char* filename) {
	XML_Document* xml_document = new XML_Document;
	if(xml_document->LoadFromFile(filename) == false) {
		delete xml_document;
		return false;
	}

	XML_TreeNode* head_node = xml_document->GetHeadNode();

	vector<XML_TreeNode*> mesh_command_lists;
	head_node->GetTreeNodesOfTagName("commandlist", mesh_command_lists);

	for(unsigned int i=0; i<mesh_command_lists.size(); i++) {
		XML_TreeNode* mesh_command_list = mesh_command_lists[i];

		vector<XML_Tag*> mesh_command_tags;
		mesh_command_list->GetOpenCloseTags(mesh_command_tags);

		for(unsigned int j=0; j<mesh_command_tags.size(); j++) {
			XML_Tag* mesh_command_tag = mesh_command_tags[j];

			MesherCommand* mc = new MesherCommand;
			if(mc->LoadFromTagData(mesh_command_tag) == true)
				AppendMesherCommand(mc);

			//printf("Command type: %s\n", mesh_command_tag->GetAttributeValue("type").c_str());
		}
	}

	delete xml_document;
	return true;
}

unsigned int TriangleMesher::GetMesherCommandCount() {
	return mesher_command_stack.size();
}

MesherCommand* TriangleMesher::GetMesherCommand(unsigned int index) {
	if(index >= GetMesherCommandCount())
		return NULL;

	return mesher_command_stack[index];
}

int TriangleMesher::AppendMesherCommand(MesherCommand* mesher_command) {
	if(mesher_command == NULL)
		return false;

	mesher_command_stack.push_back(mesher_command);

	return true;
}

int TriangleMesher::SetMesherCommand(unsigned int index, MesherCommand* mesher_command) {
	if(mesher_command == NULL || index >= GetMesherCommandCount())
		return false;

	mesher_command_stack[index] = mesher_command;
	return true;
}

int TriangleMesher::ClearMesherCommands() {
	for(unsigned int i=0; i<GetMesherCommandCount(); i++) {
		MesherCommand* mc = GetMesherCommand(i);

		if(mc != NULL)
			delete mc;
	}

	mesher_command_stack.clear();
	return true;
}

//This lets you choose whether to stop/continue if one command fails
int TriangleMesher::SetFailBehaviour(int mesher_command_fail_behaviour) {
	this->mesher_command_fail_behaviour = mesher_command_fail_behaviour;

	return true;
}

//Once all the commands have been run, their results are populated
int TriangleMesher::GetMesherCommandResult(unsigned int index) {
	if(index >= GetMesherCommandCount() || GetMesherCommand(index) == NULL)
		return -1;

	return GetMesherCommand(index)->mesher_command_result;
}

//This actually runs all of the mesher commands you set
int TriangleMesher::RunMesherCommands() {
	for(unsigned int i=0; i<GetMesherCommandCount(); i++) {
		MesherCommand* mc = GetMesherCommand(i);

		if(mc != NULL) {
			printf("\nRunning command %u\n", i);
			mc->print();
			printf("\n");

			int ret = RunMesherCommand(mc);

			if(ret == false && mesher_command_fail_behaviour == TriangleMesher::QUIT_ON_FAILURE)
				return false;
		}
	}

	return true;
}

//Run a mesher command
int TriangleMesher::RunMesherCommand(MesherCommand* mc) {
	int ret = false;

	//Figure out which command to execute and run it
	if(mc->command_type == MesherCommand::GENERATE_RANDOM_GRID)
		ret = GenerateRandomGrid(mc->xmin, mc->xmax, mc->ymin, mc->ymax, mc->vertex_count);

	else if(mc->command_type == MesherCommand::GENERATE_UNIFORM_GRID)
		ret = GenerateUniformGrid(mc->xmin, mc->xmax, mc->ymin, mc->ymax, mc->xcount, mc->ycount);

	else if(mc->command_type == MesherCommand::GENERATE_HEX_GRID)
		ret = GenerateHexGrid(mc->xmin, mc->xmax, mc->ymin, mc->ymax, mc->xcount, mc->ycount);

	else if(mc->command_type == MesherCommand::RUN_TRIANGLE_MESHER)
		ret = RunTriangleMesher(mc->use_kd_tree);

	else if(mc->command_type == MesherCommand::LOAD_MESH_FROM_FILE)
		ret = LoadMeshFromFile(mc->filename, mc->load_save_triangles);

	else if(mc->command_type == MesherCommand::SAVE_MESH_TO_FILE)
		ret = SaveMeshToFile(mc->filename, mc->load_save_triangles);

	else if(mc->command_type == MesherCommand::WRITE_SVG)
		ret = WriteSVG(mc->svg_filename, mc->svg_width, mc->svg_height);

	else if(mc->command_type == MesherCommand::SUBDIVIDE_TRIANGLE)
		ret = SubdivideTriangle(mc->subdivide_vindex, mc->subdivide_triangle_local_index);

	else if(mc->command_type == MesherCommand::BARYCENTRIC_SUBDIVIDE)
		ret = BarycentricSubdivide(mc->subdivide_triangle_local_index);

	else if(mc->command_type == MesherCommand::BASIC_TRIANGLE_MESHER)
		ret = BasicTriangleMesher();

	else if(mc->command_type == MesherCommand::STRETCHED_GRID)
		ret = StretchedGrid(mc->stretched_grid_iterations, mc->stretched_grid_alpha);

	else if(mc->command_type == MesherCommand::REFINE_MESH)
		ret = RefineMesh(mc->desired_edge_length);

	else if(mc->command_type == MesherCommand::DO_NOTHING)
		ret = true;


	//Update the mesher command result
	if(ret == false) {
		mc->mesher_command_result = MesherCommand::FAILURE_RESULT;
		return false;
	}

	else {
		mc->mesher_command_result = MesherCommand::SUCCESS_RESULT;
		return true;
	}

	return true;
}


//Debugging function
int TriangleMesher::PrintMesherCommands() {
	char buffer[1000];
	sprintf(buffer, "There are %u mesher commands on the stack", GetMesherCommandCount());
	printf("%s\n", buffer);

	for(int i=0; i<strlen(buffer); i++)
		printf("-");
	printf("\n");

	for(unsigned int i=0; i<GetMesherCommandCount(); i++) {
		MesherCommand* mc = GetMesherCommand(i);

		mc->print();
		printf("\n");
	}

	return true;
}

//////////////////////////////
// Triangle Mesher Commands //
//////////////////////////////

int TriangleMesher::GenerateRandomGrid(double xmin, double xmax, double ymin, double ymax, unsigned int vertex_count) {
	int ret = triangle_complex->GenerateRandomGrid(xmin, xmax, ymin, ymax, vertex_count);

	return ret;
}

int TriangleMesher::GenerateUniformGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount) {
	int ret = triangle_complex->GenerateUniformGrid(xmin, xmax, ymin, ymax, xcount, ycount);

	return true;
}

int TriangleMesher::GenerateHexGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount) {
	int ret = triangle_complex->GenerateHexGrid(xmin, xmax, ymin, ymax, xcount, ycount);

	return true;
}

int TriangleMesher::RunTriangleMesher(int UseKdTree) {
	int ret = triangle_complex->RunTriangleMesher();

	return ret;
}

int TriangleMesher::LoadMeshFromFile(const char* filename, int load_triangles) {
	if(global_mesh_data->LoadFromFile(filename, load_triangles) == false)
		return false;

	if(triangle_complex->AppendAllGlobalMeshData() == false)
		return false;

	return true;
}

int TriangleMesher::SaveMeshToFile(const char* filename, int save_triangles) {
	int ret = global_mesh_data->SaveToFile(filename, save_triangles);

	return ret;
}

int TriangleMesher::WriteSVG(const char* filename, double svg_width, double svg_height) {
	int ret = global_mesh_data->WriteSVG(filename, svg_width, svg_height);

	return ret;
}

int TriangleMesher::AppendVertex(Vector2d vertex) {
	return true;
}

int TriangleMesher::SubdivideTriangle(unsigned int vindex, unsigned int triangle_local_index) {
	int ret = triangle_complex->SubdivideTriangle(vindex, triangle_local_index);

	return ret;
}

int TriangleMesher::BarycentricSubdivide(unsigned int triangle_local_index) {
	int ret = triangle_complex->BarycentricSubdivide(triangle_local_index);

	return ret;
}

int TriangleMesher::BasicTriangleMesher() {
	int ret = triangle_complex->BasicTriangleMesher();

	return ret;
}

int TriangleMesher::BasicDelaunayFlipper(unsigned int delaunay_max_iterations) {
	return true;
}

int TriangleMesher::StretchedGrid(unsigned int iterations, double alpha) {
	int ret = triangle_complex->StretchedGridMethod(iterations, alpha);

	return ret;
}

int TriangleMesher::RefineMesh(double desired_edge_length) {
	int ret = triangle_complex->AdjustCellEdgeLength(desired_edge_length);

	return ret;
}

/////////////////////
// Data management //
/////////////////////

TriangleComplex* TriangleMesher::GetTriangleComplex() {
	return triangle_complex;
}
