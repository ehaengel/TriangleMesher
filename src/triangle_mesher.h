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
#include "triangle_complex.h"
#include "mesher_command.h"

//Mesh data code
#include "global_mesh_data.h"

#ifndef TRIANGLE_MESHER
#define TRIANGLE_MESHER

class TriangleMesher {
public:
	TriangleMesher();
	~TriangleMesher();

	////////////////////////////////////////
	// Triangle Mesher Command Management //
	////////////////////////////////////////

	//These functions are used to append/remove commands to run later
	int LoadCommandsFromFile(const char* filename);

	unsigned int GetMesherCommandCount();
	MesherCommand* GetMesherCommand(unsigned int index);

	int AppendMesherCommand(MesherCommand* mesher_command);
	int SetMesherCommand(unsigned int index, MesherCommand* mesher_command);

	int ClearMesherCommands();

	//This lets you choose whether to stop/continue if one command fails
	int SetFailBehaviour(int mesher_command_fail_behaviour);
	enum {
		QUIT_ON_FAILURE=0,
		CONTINUE_ON_FAILURE
	};

	//Once all the commands have been run, their results are populated
	int GetMesherCommandResult(unsigned int index);

	//This actually runs all of the mesher commands you set
	int RunMesherCommands();

	//Run a mesher command
	int RunMesherCommand(MesherCommand* mc);

	//Debugging function
	int PrintMesherCommands();

	//////////////////////////////
	// Triangle Mesher Commands //
	//////////////////////////////

	int GenerateRandomGrid(double xmin, double xmax, double ymin, double ymax, unsigned int vertex_count);
	int GenerateUniformGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount);
	int GenerateHexGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount);

	int RunTriangleMesher(int UseKdTree);

	int LoadMeshFromFile(const char* filename);
	int SaveMeshToFile(const char* filename);

	int WriteSVG(const char* filename, double svg_width, double svg_height);

	int AppendVertex(Vector2d vertex);

	int SubdivideTriangle(unsigned int vindex, unsigned int triangle_local_index);
	int BarycentricSubdivide(unsigned int triangle_local_index);

	int BasicTriangleMesher();
	int BasicDelaunayFlipper(unsigned int delaunay_max_iterations);

	int StretchedGrid(unsigned int iterations, double alpha);

	int RefineMesh(double desired_edge_length);

	/////////////////////
	// Data management //
	/////////////////////

	TriangleComplex* GetTriangleComplex();

private:
	//Data regarding the mesher command stack
	vector<MesherCommand*> mesher_command_stack;

	int mesher_command_fail_behaviour;

	//The triangle complex this mesher operates on
	TriangleComplex* triangle_complex;
};

#endif
