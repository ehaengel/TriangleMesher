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

#ifndef TRIANGLE_MESHER
#define TRIANGLE_MESHER

class TriangleMesher {
public:
	TriangleMesher();
	~TriangleMesher();

private:
	//Data regarding the mesher command stack
	vector<MesherCommand> mesher_command_stack;
	vector<int> mesher_command_results;

	int mesher_command_fail_behaviour;
	enum {
		QUIT_ON_FAILURE=0,
		CONTINUE_ON_FAILURE
	};

	//The triangle complex this mesher operates on
	TriangleComplex* triangle_complex;
};

#endif
