#include "triangle_mesher.h"

TriangleMesher::TriangleMesher() {
	//Data regarding the mesher command stack
	mesher_command_stack.clear();

	mesher_command_fail_behaviour = TriangleMesher::QUIT_ON_FAILURE;

	//The triangle complex this mesher operates on
	triangle_complex = new TriangleComplex;
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
	//To be done!!

	return true;
}

//////////////////////////////
// Triangle Mesher Commands //
//////////////////////////////

int TriangleMesher::GenerateRandomGrid(double xmin, double xmax, double ymin, double ymax, unsigned int vertex_count) {
	return true;
}

int TriangleMesher::GenerateUniformGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount) {
	return true;
}

int TriangleMesher::RunTriangleMesher(int UseKdTree) {
	return true;
}

int TriangleMesher::LoadMeshFromFile(const char* filename) {
	return true;
}

int TriangleMesher::SaveMeshToFile(const char* filename) {
	return true;
}

int TriangleMesher::AppendVertex(Vector2d vertex) {
	return true;
}

int TriangleMesher::BasicTriangleMesher() {
	return true;
}

int TriangleMesher::BasicDelaunayFlipper(unsigned int delaunay_max_iterations) {
	return true;
}

int TriangleMesher::StretchedGrid() {
	return true;
}
