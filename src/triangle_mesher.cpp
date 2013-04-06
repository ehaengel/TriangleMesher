#include "triangle_mesher.h"

TriangleMesher::TriangleMesher() {
	//Data regarding the mesher command stack
	mesher_command_stack.clear();
	mesher_command_results.clear();

	mesher_command_fail_behaviour = TriangleMesher::QUIT_ON_FAILURE;

	//The triangle complex this mesher operates on
	triangle_complex = NULL;
}

TriangleMesher::~TriangleMesher() {
}
