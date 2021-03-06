#include "triangle_complex.h"

TriangleComplex::TriangleComplex() {
	//Create the global mesh data
	global_mesh_data = new GlobalMeshData;

	//Initialize all the other variables
	initialize();
}

TriangleComplex::TriangleComplex(GlobalMeshData* global_mesh_data) {
	//Get the global mesh data from the function input
	this->global_mesh_data = global_mesh_data;

	//Initialize all the other variables
	initialize();
}

TriangleComplex::~TriangleComplex() {
	free_data();
}

//General use functions
/*int TriangleComplex::LoadFromFile(const char* filename) {
	//First free up all the old data
	free_data();
	initialize();

	//Reset the global vertex list
	//reset_global_vertex_list();

	//Open the mesh file as an XML document
	XML_Document* xml_document = new XML_Document;
	if(xml_document->LoadFromFile(filename) == false) {
		printf("Error loading the mesh file\n");

		delete xml_document;
		return false;
	}

	//Load in the vertices from the mesh file
	vector<XML_TreeNode*> vertexlists;
	xml_document->GetHeadNode()->GetTreeNodesOfTagName("vertexlist", vertexlists);

	for(unsigned int i=0; i<vertexlists.size(); i++) {
		XML_TreeNode* vertexlist = vertexlists[i];

		vector<XML_Tag*> new_vertex_tags;
		vertexlist->GetTagsOfTagName("vertex", new_vertex_tags);

		for(unsigned int j=0; j<new_vertex_tags.size(); j++) {
			XML_Tag* new_vertex_tag = new_vertex_tags[j];

			//Get the vertex information from this tag
			string index_str = new_vertex_tag->GetAttributeValue("index");
			if(index_str == "") {
				printf("HERE!!\n");
				delete xml_document;
				return false;
			}

			unsigned int index = (unsigned int) atoi(index_str.c_str());
			//if(index < global_vertex_list->size()) {
			if(index < global_mesh_data->GetVertexCount()) {
				Vector2d* pt = GetGlobalVertex(index);

				if(pt != NULL) {
					printf("The same vertex is defined twice\n");

					delete xml_document;
					return false;
				}
			}

			string vx_str = new_vertex_tag->GetAttributeValue("x");
			if(vx_str == "") {
				delete xml_document;
				return false;
			}

			string vy_str = new_vertex_tag->GetAttributeValue("y");
			if(vy_str == "") {
				delete xml_document;
				return false;
			}

			Vector2d* new_vertex = new Vector2d;
			new_vertex->x = atof(vx_str.c_str());
			new_vertex->y = atof(vy_str.c_str());

			//new_vertices[index] = new_vertex;
			//if(global_vertex_list->size() <= index)
			//	global_vertex_list->resize(index+1, NULL);

			//(*global_vertex_list)[index] = new_vertex;

			global_mesh_data->SetVertex(index, new_vertex);
			AppendVertexIndex(index);
		}
	}

	printf("Starting to load triangles\n");
	//Load in the triangles from the mesh file
	vector<XML_TreeNode*> trianglelists;
	xml_document->GetHeadNode()->GetTreeNodesOfTagName("trianglelist", trianglelists);

	unsigned int cur_triangle_count = GetTriangleCount();
	unsigned int cur_triangles_loaded = 0;

	unsigned int total_triangles = GetTriangleCount();
	for(unsigned int i=0; i<trianglelists.size(); i++) {
		XML_TreeNode* trianglelist = trianglelists[i];

		vector<XML_Tag*> new_triangle_tags;
		total_triangles += trianglelist->CountTagsOfTagName("triangle");
	}
	//triangle_list->resize(total_triangles, NULL);

	for(unsigned int i=0; i<trianglelists.size(); i++) {
		XML_TreeNode* trianglelist = trianglelists[i];

		vector<XML_Tag*> new_triangle_tags;
		trianglelist->GetTagsOfTagName("triangle", new_triangle_tags);

		for(unsigned int j=0; j<new_triangle_tags.size(); j++) {
			XML_Tag* triangle_tag = new_triangle_tags[j];

			unsigned int index = 0;
			string index_str = triangle_tag->GetAttributeValue("index");
			if(index_str != "") {
				index = (unsigned int) atoi(index_str.c_str());

				if(index == 0 || index >= total_triangles || GetTriangle(index) != NULL) {
					printf("FAILING HERE!\n");

					delete xml_document;
					return false;
				}
			}

			unsigned int n0 = 0;
			unsigned int n1 = 0;
			unsigned int n2 = 0;

			string n0_str = triangle_tag->GetAttributeValue("n0");
			if(n0_str != "") n0 = (unsigned int) atoi(n0_str.c_str());

			string n1_str = triangle_tag->GetAttributeValue("n1");
			if(n1_str != "") n1 = (unsigned int) atoi(n1_str.c_str());

			string n2_str = triangle_tag->GetAttributeValue("n2");
			if(n2_str != "") n2 = (unsigned int) atoi(n2_str.c_str());

			if(n0 == 0 || n1 == 0 || n2 == 0) {
				printf("NO HERE!!\n");

				delete xml_document;
				return false;
			}

			//Now that all the data is loaded, create a new triangle
			Triangle* new_tri = new Triangle(global_mesh_data->GetGlobalVertexList());

			new_tri->SetLocalIndex(index);

			new_tri->SetVertex(0, n0);
			new_tri->SetVertex(1, n1);
			new_tri->SetVertex(2, n2);

			//SetTriangle(cur_triangle_count + cur_triangles_loaded, new_tri);
			global_mesh_data->SetTriangle(index, new_tri);
			AppendTriangleIndex(index);
			cur_triangles_loaded++;
		}
	}

	printf("Starting to load adjacency\n");
	//Load in the triangle adjacency information
	for(unsigned int i=0; i<trianglelists.size(); i++) {
		XML_TreeNode* trianglelist = trianglelists[i];

		vector<XML_Tag*> new_triangle_tags;
		trianglelist->GetTagsOfTagName("triangle", new_triangle_tags);

		for(unsigned int j=0; j<new_triangle_tags.size(); j++) {
			XML_Tag* triangle_tag = new_triangle_tags[j];

			unsigned int index = 0;
			string index_str = triangle_tag->GetAttributeValue("index");
			if(index_str != "")
				index = (unsigned int) atoi(index_str.c_str());

			unsigned int a0 = 0;
			unsigned int a1 = 0;
			unsigned int a2 = 0;

			string a0_str = triangle_tag->GetAttributeValue("a0");
			if(a0_str != "") a0 = (unsigned int) atoi(a0_str.c_str());

			string a1_str = triangle_tag->GetAttributeValue("a1");
			if(a1_str != "") a1 = (unsigned int) atoi(a1_str.c_str());

			string a2_str = triangle_tag->GetAttributeValue("a2");
			if(a2_str != "") a2 = (unsigned int) atoi(a2_str.c_str());

			if(a0 >= total_triangles || a1 >= total_triangles || a2 >= total_triangles) {
				delete xml_document;
				return false;
			}

			Triangle* tri = GetGlobalTriangle(index);
			if(tri == NULL) {
				delete xml_document;
				return false;
			}

			Triangle* adj_tri0 = NULL;
			Triangle* adj_tri1 = NULL;
			Triangle* adj_tri2 = NULL;

			adj_tri0 = GetGlobalTriangle(a0);
			adj_tri1 = GetGlobalTriangle(a1);
			adj_tri2 = GetGlobalTriangle(a2);

			tri->SetAdjacentTriangle(0, adj_tri0);
			tri->SetAdjacentTriangle(1, adj_tri1);
			tri->SetAdjacentTriangle(2, adj_tri2);
		}
	}

	printf("Fixing adjacencies\n");
	//Fix the triangle adjacencies
	if(compute_triangle_adjacencies() == false) {
		delete xml_document;
		return false;
	}

	printf("DONE loading file\n");
	delete xml_document;
	return true;
}*/

/*int TriangleComplex::WriteToFile(const char* filename) {
	//Some simple error checking
	//if(global_vertex_list->size() <= 1)
	//	return false;

	//Open the output file as an xml document
	XML_Document* xml_document = new XML_Document;

	XML_TreeNode* head_node = xml_document->GetHeadNode();

	//Create a vertexlist
	XML_TreeNode* vertexlist_node = head_node->CreateChildTagPair("vertexlist");

	//Write the vertices to the file
	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* pt = GetVertex(i);

		if(pt == NULL)
			continue;

		XML_TreeNode* vertex_node = vertexlist_node->CreateChildTagOpenClosed("vertex");

		XML_Tag* vertex_node_tag = vertex_node->GetStartTag();
		vertex_node_tag->AppendTagAttribute("index", GetVertexIndex(i));
		vertex_node_tag->AppendTagAttribute("x", pt->x);
		vertex_node_tag->AppendTagAttribute("y", pt->y);
	}

	//Create a trianglelist
	XML_TreeNode* trianglelist_node = head_node->CreateChildTagPair("trianglelist");

	//Write the triangles to the file
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);

		if(tri == NULL)
			continue;

		XML_TreeNode* triangle_node = trianglelist_node->CreateChildTagOpenClosed("triangle");
		XML_Tag* triangle_node_tag = triangle_node->GetStartTag();

		triangle_node_tag->AppendTagAttribute("index", tri->GetLocalIndex());

		for(int j=0; j<3; j++) {
			char buffer[1000];
			sprintf(buffer, "n%d", j);

			triangle_node_tag->AppendTagAttribute(string(buffer), tri->GetVertexIndex(j));
		}

		for(int j=0; j<3; j++) {
			char buffer[1000];
			sprintf(buffer, "a%d", j);

			if(tri->GetAdjacentTriangle(j) == NULL)
				triangle_node_tag->AppendTagAttribute(string(buffer), 0);

			else {
				unsigned int adj_local_index = tri->GetAdjacentTriangle(j)->GetLocalIndex();
				triangle_node_tag->AppendTagAttribute(string(buffer), adj_local_index);
			}
		}
	}

	int ret = xml_document->WriteToFile(filename);

	delete xml_document;
	return ret;
}*/

///////////////////////////////
// Data management functions //
///////////////////////////////

//Data management for the global mesh
GlobalMeshData* TriangleComplex::GetGlobalMeshData() {
	return global_mesh_data;
}

int TriangleComplex::AppendAllGlobalMeshData() {
	if(AppendAllVertexIndices() == false)
		return false;

	if(AppendAllTriangleIndices() == false)
		return false;

	return true;
}

//Data management for triangles
unsigned int TriangleComplex::GetTriangleCount() {
	return triangle_list.size();
}

unsigned int TriangleComplex::GetTriangleIndex(unsigned int triangle) {
	if(triangle >= GetTriangleCount())
		return 0;

	return triangle_list[triangle];
}

Triangle* TriangleComplex::GetTriangle(unsigned int triangle) {
	unsigned int tindex = GetTriangleIndex(triangle);
	return global_mesh_data->GetTriangle(tindex);
}

Triangle* TriangleComplex::GetGlobalTriangle(unsigned int tindex) {
	return global_mesh_data->GetTriangle(tindex);
}

int TriangleComplex::SetTriangleIndex(unsigned int triangle, unsigned int tindex) {
	if(triangle >= GetTriangleCount())
		return false;

	triangle_list[triangle] = tindex;
	return true;
}

int TriangleComplex::AppendTriangleIndex(unsigned int tindex) {
	triangle_list.push_back(tindex);
	return true;
}

int TriangleComplex::AppendAllTriangleIndices() {
	//First remove all triangles from this complex
	RemoveAllTriangles();

	//Then add all the triangles from the global mesh
	for(unsigned int i=0; i<global_mesh_data->GetTriangleCount(); i++) {
		Triangle* tri = global_mesh_data->GetTriangle(i);
		if(tri != NULL)
			triangle_list.push_back(i);
	}

	return true;
}

int TriangleComplex::SetTriangle(unsigned int tindex, Triangle* tri) {
	return global_mesh_data->SetTriangle(tindex, tri);
}

unsigned int TriangleComplex::AppendTriangle(Triangle* tri) {
	unsigned int tindex = global_mesh_data->AppendTriangle(tri);
	AppendTriangleIndex(tindex);
}

int TriangleComplex::RemoveTriangle(unsigned int triangle) {
	if(triangle >= GetTriangleCount())
		return false;

	triangle_list.erase(triangle_list.begin() + triangle);
	return true;
}

int TriangleComplex::RemoveAllTriangles() {
	printf("Clearing out the triangle list!!\n");

	triangle_list.clear();
	return true;
}

int TriangleComplex::DeleteTriangle(unsigned int tindex) {
	return global_mesh_data->DeleteTriangle(tindex);

	return true;
}

//Data management for vertices
unsigned int TriangleComplex::GetVertexCount() {
	return vertex_list.size();
}

unsigned int TriangleComplex::GetVertexIndex(unsigned int vertex) {
	//Safety check
	if(vertex >= GetVertexCount())
		return 0;

	return vertex_list[vertex];
}

Vector2d* TriangleComplex::GetVertex(unsigned int vertex) {
	//Safety check
	if(vertex >= GetVertexCount())
		return NULL;

	unsigned int vindex = GetVertexIndex(vertex);
	return global_mesh_data->GetVertex(vindex);
	//return (*global_vertex_list)[vindex];
}

Vector2d* TriangleComplex::GetGlobalVertex(unsigned int vindex) {
	return global_mesh_data->GetVertex(vindex);
}

int TriangleComplex::SetVertexIndex(unsigned int vertex, unsigned int vindex) {
	//Safety check
	if(vertex >= GetVertexCount())
		return false;

	vertex_list[vertex] = vindex;
	return true;
}

int TriangleComplex::AppendVertexIndex(unsigned int vindex) {
	vertex_list.push_back(vindex);
	return true;
}

int TriangleComplex::AppendAllVertexIndices() {
	//First remove all the vertices from this complex
	RemoveAllVertices();

	//Then add all the vertices from the global mesh
	for(unsigned int i=0; i<global_mesh_data->GetVertexCount(); i++) {
		Vector2d* v = global_mesh_data->GetVertex(i);
		if(v != NULL)
			vertex_list.push_back(i);
	}

	return true;
}

int TriangleComplex::RemoveVertex(unsigned int vertex) {
	if(vertex >= GetVertexCount())
		return false;

	vertex_list.erase(vertex_list.begin() + vertex);
	return true;
}

int TriangleComplex::RemoveAllVertices() {
	printf("Clearing out the vertex list!!\n");

	vertex_list.clear();
	return true;
}

//Generate some regular/random vertex sets
int TriangleComplex::GenerateRandomGrid(double xmin, double xmax, double ymin, double ymax, unsigned int vertex_count) {
	//Safety test
	if(xmin >= xmax || ymin >= ymax || vertex_count == 0)
		return false;

	for(unsigned int i=0; i<vertex_count; i++) {
		Vector2d* new_vertex = new Vector2d;
		new_vertex->set(get_rand(xmin, xmax), get_rand(ymin, ymax));

		unsigned int vindex = global_mesh_data->AppendVertex(new_vertex);
		AppendVertexIndex(vindex);
		//global_vertex_list->push_back(new_vector);
		//AppendVertexIndex(global_vertex_list->size()-1);
	}

	return true;
}

int TriangleComplex::GenerateUniformGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount) {
	//Safety test
	if(xmin >= xmax || ymin >= ymax || xcount == 0 || ycount == 0)
		return false;

	double xbuf = xmin;
	double dx = 0.0;
	//if(xcount > 1) dx = (xmax - xmin) / (double(xcount) - 0.5);
	if(xcount > 1) dx = (xmax - xmin) / double(xcount - 1);

	double ybuf = ymin;
	double dy = 0.0;
	if(ycount > 1) dy = (ymax - ymin) / double(ycount - 1);

	for(unsigned int i=0; i<ycount; i++) {
		//if(i % 2 == 0)	xbuf = xmin;
		//else			xbuf = xmin + 0.5*dx;
		xbuf = xmin;

		for(unsigned int j=0; j<xcount; j++) {
			Vector2d* new_vertex = new Vector2d(xbuf, ybuf);

			unsigned int vindex = global_mesh_data->AppendVertex(new_vertex);
			AppendVertexIndex(vindex);
			//global_vertex_list->push_back(new_vector);
			//AppendVertexIndex(global_vertex_list->size()-1);

			xbuf += dx;
		}

		ybuf += dy;
	}

	return true;
}

int TriangleComplex::GenerateHexGrid(double xmin, double xmax, double ymin, double ymax, unsigned int xcount, unsigned int ycount) {
	//Safety test
	if(xmin >= xmax || ymin >= ymax || xcount == 0 || ycount == 0)
		return false;

	Vector2d start(0.5*(xmin + xmax), 0.5*(ymin + ymax));

	double xlen = (1/cos(15*3.141592/180.0))*(xmax - xmin) / (xcount - 1);
	double ylen = (1/cos(15*3.141592/180.0))*(ymax - ymin) / (ycount - 1);

	Vector2d u(xlen*cos(15*3.141592/180.0), xlen*sin(15*3.141592/180.0));
	Vector2d v(ylen*sin(15*3.141592/180.0), ylen*cos(15*3.141592/180.0));

	for(int i=-int(ycount); i<int(ycount); i++) {
		for(int j=-int(xcount); j<int(xcount); j++) {
			Vector2d temp = start + (u*i) + (v*j);
			if(temp.x >= xmin && temp.x <= xmax && temp.y >= ymin && temp.y <= ymax) {
				Vector2d* new_vertex = new Vector2d;
				*new_vertex = temp;

				unsigned int vindex = global_mesh_data->AppendVertex(new_vertex);
				AppendVertexIndex(vindex);
				//global_vertex_list->push_back(new_vector);
				//AppendVertexIndex(global_vertex_list->size()-1);
			}
		}
	}

	return true;
}

vector<unsigned int> TriangleComplex::GetIncompleteVertices() {
	return incomplete_vertices;
}

vector<double> TriangleComplex::GetIncompleteVerticesAngles() {
	return incomplete_vertices_angles;
}

int TriangleComplex::SetIncompleteListsComputed(int incomplete_lists_computed) {
	this->incomplete_lists_computed = incomplete_lists_computed;

	return true;
}

//Meshing functions
int TriangleComplex::RunTriangleMesher() {
	//If this is the kd_parent
	//if(kd_parent == NULL && global_vertex_list->size() >= MAXIMUM_MESH_SIZE) {
	if(kd_parent == NULL && GetVertexCount() >= MAXIMUM_MESH_SIZE) {
		//Keep track of the number of runs
		char filename[1000];
		int run_count = 0;

		printf("MESHING THE PARENT NODE!!\n");
		if(CreateKDTree() == false)
			return false;

		printf("DONE FOREVER WITH CREATING THE KD-TREE\n\n");

		while(kd_leaf_nodes->size() > 1) {
			//Mesh all the leaf nodes

			#pragma omp parallel for
			for(int i=0; i<kd_leaf_nodes->size(); i++) {
				printf("Starting to mesh leaf node %u\n", i);
				(*kd_leaf_nodes)[i]->RunTriangleMesher();

				printf("Done meshing leaf node %u\n\n", i);
			}

			//Write an output file
			//sprintf(filename, "out%d.svg", run_count++);
			//write_svg(filename, 1000, 1000);

			//Combine sibling leaf-nodes
			vector<TriangleComplex*> new_kd_leaf_nodes;
			new_kd_leaf_nodes.clear();

			printf("Starting with %u kd leaf nodes\n", kd_leaf_nodes->size());
			int done = false;
			while(kd_leaf_nodes->size() > 1) {
				TriangleComplex* tc_parent = (*kd_leaf_nodes)[0]->GetKDParent();

				//Safety check
				if(tc_parent == NULL) {
					printf("OH NOES\n");
					return false;
				}

				//Combine the children to create a new leaf node
				// + This will delete the children from kd_leaf_nodes
				if(tc_parent->CombineChildren() == false) {
					printf("No children!!\n");
					new_kd_leaf_nodes.push_back((*kd_leaf_nodes)[0]);
					kd_leaf_nodes->erase(kd_leaf_nodes->begin());

					//printf("FAILED TO COMBINE CHILDREN: %u\n", kd_leaf_nodes->size());
					//return false;
				}
				else
					new_kd_leaf_nodes.push_back(tc_parent);
			}
			if(done == true)
				break;

			*kd_leaf_nodes = new_kd_leaf_nodes;
			printf("KD LEAF NODE COUNT: %u\n", kd_leaf_nodes->size());

			//Write an output file
			//sprintf(filename, "out%d.svg", run_count++);
			//write_svg(filename, 1000, 1000);
		}

		CombineChildren();

		if(basic_triangle_mesher() == false)
			return false;

		if(basic_delaunay_flipper() == false)
			return false;

		//Clean up the mesh
		//if(basic_mesh_cleaner() == false)
		//	return false;

		//Reset the local indices of all triangles
		unsigned int index = 1;
		for(unsigned int i=0; i<GetTriangleCount(); i++) {
			Triangle* tri = GetTriangle(i);

			if(tri != NULL)
				tri->SetLocalIndex(index++);
		}
	}

	//Otherwise
	else {
		if(basic_triangle_mesher() == false)
			return false;

		if(basic_delaunay_flipper() == false)
			return false;

		if(kd_parent == NULL) {
			//Clean up the mesh
			//if(basic_mesh_cleaner() == false)
			//	return false;

			//Reset the local indices of all triangles
			unsigned int index = 1;
			for(unsigned int i=0; i<GetTriangleCount(); i++) {
				Triangle* tri = GetTriangle(i);

				if(tri != NULL)
					tri->SetLocalIndex(index++);
			}
		}
	}

	return true;
}

int TriangleComplex::RunDelaunayFlips() {
	if(basic_delaunay_flipper() == false)
		return false;

	return true;
}

int TriangleComplex::BasicTriangleMesher() {
	if(basic_triangle_mesher() == false)
		return false;

	return true;
}

int TriangleComplex::SubdivideTriangle(unsigned int vindex, unsigned int triangle_local_index) {
	int found = false;
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);

		if(tri != NULL && tri->GetLocalIndex() == triangle_local_index) {
			found = true;

			//Try to subdivide this triangle
			vector<Triangle*> results;

			if(tri->SubdivideTriangle(vindex, results) == false)
				return false;

			//Add the vindex to this complex if it isn't already in there
			int vindex_in_complex = false;
			for(unsigned int j=0; j<GetVertexCount(); j++) {
				if(GetVertexIndex(j) == vindex) {
					vindex_in_complex = true;
					break;
				}
			}
			if(vindex_in_complex == false)
				AppendVertexIndex(vindex);

			//And replace this triangle with its subdivision
			DeleteTriangle(i);
			AppendTriangle(results[0]);
			AppendTriangle(results[1]);
			AppendTriangle(results[2]);
		}
	}

	return found;
}

int TriangleComplex::BarycentricSubdivide(unsigned int triangle_local_index) {
	int found = false;
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);

		if(tri != NULL && tri->GetLocalIndex() == triangle_local_index) {
			found = true;

			//Try to barycentric subdivide this triangle
			unsigned int centroid_vindex = 0;
			vector<Triangle*> results;

			if(tri->BarycentricSubdivide(centroid_vindex, results) == false)
				return false;

			//Add the centroid vindex to this complex
			AppendVertexIndex(centroid_vindex);

			//And replace this triangle with its subdivision
			DeleteTriangle(i);
			AppendTriangle(results[0]);
			AppendTriangle(results[1]);
			AppendTriangle(results[2]);
		}
	}

	return found;
}

int TriangleComplex::StretchedGridMethod(unsigned int iterations, double alpha) {
	if(force_stretched_grid_method(iterations, alpha) == false)
		return false;

	//if(basic_stretched_grid_method(iterations, alpha) == false)
	//	return false;

	return true;
}

int TriangleComplex::AdjustCellEdgeLength(double desired_edge_length) {
	unsigned int edge_count;
	double current_cell_edge_length = 0.0;

	printf("RUNNING MESH REFINEMENT %f\n", desired_edge_length);

	//Compute the current average cell edge length
	if(ComputeEdgeStatistics(edge_count, current_cell_edge_length) == false)
		return false;

	printf("Current cell edge length: %f\n", current_cell_edge_length);

	//Safety test
	if(edge_count < 3)
		return false;

	//Refine the mesh if it is not dense enough
	if(current_cell_edge_length > desired_edge_length)
		if(refine_mesh(desired_edge_length, edge_count, current_cell_edge_length) == false)
			return false;

	//Coarsify the mesh if it is too dense
	else {
		//Do nothing
	}

	printf("\n");
	return true;
}


/////////////////////////////
// Mesh Geometry functions //
/////////////////////////////

//This functon fills a list with triangles which overlap a prism
int TriangleComplex::GetTrianglesInsidePrism(vector<Triangle*> &result, Prism p) {
	result.clear();
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);

		if(tri != NULL && prism_triangle_intersection_closed(p, *tri) == true)
			result.push_back(tri);
	}

	return true;
}

//This function creates a list with one integer per triangle
// + this integer is 0 if the corresponding triangle from GetTriangle(...) does not overlap p
// + this integer is 1 if the corresponding triangle from GetTriangle(...) does overlap p
int TriangleComplex::GetTrianglesInsidePrism(vector<int> &result, Prism p) {
	result.clear();
	result.resize(GetTriangleCount(), false);

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);

		if(tri != NULL && prism_triangle_intersection_closed(p, *tri) == true)
			result[i] = true;
	}

	return true;
}

//This function creates a list of edges for the whole complex
int TriangleComplex::GetEdges(vector<Edge*> &result) {
	result.clear();

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri == NULL)
			continue;

		for(int j=0; j<3; j++) {
			Edge* e = NULL;
			tri->GetOpposingEdge(e, j);

			//Make sure this is a good edge
			if(e->IsGoodEdge() == false)
				continue;

			//Look through the results to put it in the right spot
			int inserted = false;
			for(unsigned int k=0; k<result.size(); k++) {
				//If the edge is in the list already, skip it
				if((*result[k]) == (*e)) {
					delete e;
					inserted = true;
					break;
				}

				//We have found the right insertion point
				if((*e) < (*result[k])) {
					result.insert(result.begin() + k, e);
					inserted = true;
					break;
				}
			}
			//If the edge was not found already and has yet to be inserted, put it at the end of the list
			if(inserted == false)
				result.push_back(e);
		}
	}

	return true;
}

//This function creates a list of edges overlapping a prism
int TriangleComplex::GetEdgesInsidePrism(vector<Edge*> &result, Prism p) {
	result.clear();

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri == NULL)
			continue;

		for(int j=0; j<3; j++) {
			Edge* e = NULL;
			tri->GetOpposingEdge(e, j);

			//Make sure this is a good edge
			if(e->IsGoodEdge() == false)
				continue;

			Vector2d* p1 = e->GetVertex(0);
			Vector2d* p2 = e->GetVertex(1);
			if(p1 == NULL || p2 == NULL)
				continue;

			if(prism_line_segment_intersection_closed(p, *p1, *p2) == false)
				continue;

			//Look through the results to put it in the right spot
			int inserted = false;
			for(unsigned int k=0; k<result.size(); k++) {
				//If the edge is in the list already, skip it
				if((*result[k]) == (*e)) {
					delete e;
					inserted = true;
					break;
				}

				//We have found the right insertion point
				if((*e) < (*result[k])) {
					result.insert(result.begin() + k, e);
					inserted = true;
					break;
				}
			}
			//If the edge was not found already and has yet to be inserted, put it at the end of the list
			if(inserted == false)
				result.push_back(e);
		}
	}

	return true;
}


//Compute the average edge length of a list of edges
int TriangleComplex::ComputeAverageEdgeLength(double& result, vector<Edge*> edge_list) {
	result = 0.0;

	double edge_count = double(edge_list.size());
	for(unsigned int i=0; i<edge_list.size(); i++)
		result += edge_list[i]->ComputeLength() / edge_count;

	return true;
}

//Compute statistics on the edges in this complex
int TriangleComplex::ComputeEdgeStatistics(unsigned int& edge_count, double& average_edge_length) {
	edge_count = 0;
	average_edge_length = 0.0;

	//Get a list of all the edges
	vector<Edge*> edge_list;
	if(GetEdges(edge_list) == false)
		return false;

	//Do the computations here
	edge_count = edge_list.size();
	ComputeAverageEdgeLength(average_edge_length, edge_list);

	//Clean up the data
	for(unsigned int i=0; i<edge_list.size(); i++)
		delete edge_list[i];

	return true;
}

//Compute statistics on the edges overlapping a prism
int TriangleComplex::ComputeEdgeStatisticsInsidePrism(unsigned int& edge_count, double& average_edge_length, Prism p) {
	edge_count = 0;
	average_edge_length = 0.0;

	//Get a list of all the edges
	vector<Edge*> edge_list;
	if(GetEdgesInsidePrism(edge_list, p) == false)
		return false;

	//Do the computations here
	edge_count = edge_list.size();
	ComputeAverageEdgeLength(average_edge_length, edge_list);

	//Clean up the data
	for(unsigned int i=0; i<edge_list.size(); i++)
		delete edge_list[i];

	return true;
}



////////////////////////////////
// K-d tree related functions //
////////////////////////////////
int TriangleComplex::CreateKDTree() {
	//The top node creates a global kd prism
	if(kd_parent == NULL) {
		//Create the list of leaf nodes
		kd_leaf_nodes = new vector<TriangleComplex*>;

		if(compute_kd_prism() == false) {
			kd_leaf_nodes->push_back(this);
			return false;
		}
	}

	//Check the subdivision condition
	if(GetVertexCount() < MAXIMUM_MESH_SIZE) {
		kd_leaf_nodes->push_back(this);
		return true;
	}

	//Get the vertex with which to split up this triangle complex
	unsigned int splitting_index = compute_centermost_vertex(kd_splitting_dimension);
	Vector2d* vs = GetVertex(splitting_index);
	if(vs == NULL) {
		kd_leaf_nodes->push_back(this);
		return false;
	}

	printf("splitting index: %u\n", splitting_index);

	//Create two children complices
	//kd_child[0] = new TriangleComplex(global_vertex_list);
	//kd_child[1] = new TriangleComplex(global_vertex_list);
	kd_child[0] = new TriangleComplex(global_mesh_data);
	kd_child[1] = new TriangleComplex(global_mesh_data);

	//Divide up the vertices
	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* vi = GetVertex(i);
		if(vi == NULL)
			continue;

		if(kd_splitting_dimension == 0) {
			if(vi->x <= vs->x)
				kd_child[0]->AppendVertexIndex(GetVertexIndex(i));

			else
				kd_child[1]->AppendVertexIndex(GetVertexIndex(i));
		}

		else if(kd_splitting_dimension == 1) {
			if(vi->y <= vs->y)
				kd_child[0]->AppendVertexIndex(GetVertexIndex(i));

			else
				kd_child[1]->AppendVertexIndex(GetVertexIndex(i));
		}
	}

	//Divide up the triangles
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		unsigned int tindex = GetTriangleIndex(i);
		if(tri == NULL)
			continue;

		int added_to_zero = false;
		int added_to_one = false;

		for(int j=0; j<3; j++) {
			Vector2d* pt = tri->GetVertex(j);
			if(pt != NULL) {
				if(kd_splitting_dimension == 0) {
					if(added_to_zero == false && pt->x <= vs->x) {
						kd_child[0]->AppendTriangleIndex(tindex);
						//kd_child[0]->AppendTriangle(tri);
						added_to_zero = true;
					}
					else if(added_to_one == false && pt->x > vs->x) {
						kd_child[0]->AppendTriangleIndex(tindex);
						//kd_child[1]->AppendTriangle(tri);
						added_to_one = true;
					}
				}
				if(kd_splitting_dimension == 1) {
					if(added_to_zero == false && pt->y <= vs->y) {
						kd_child[0]->AppendTriangleIndex(tindex);
						//kd_child[0]->AppendTriangle(tri);
						added_to_zero = true;
					}
					else if(added_to_one == false && pt->y > vs->y) {
						kd_child[1]->AppendTriangleIndex(tindex);
						//kd_child[1]->AppendTriangle(tri);
						added_to_one = true;
					}
				}
			}
		}

		//This is a bridge triangle
		if(added_to_zero == true && added_to_one == true) {
			//kd_child[0]->AppendBridgeTriangleIndex(tri->GetLocalIndex());
			//kd_child[1]->AppendBridgeTriangleIndex(tri->GetLocalIndex());
			kd_child[0]->AppendBridgeTriangleIndex(tindex);
			kd_child[1]->AppendBridgeTriangleIndex(tindex);
		}
	}

	//Remove all the triangles from this complex
	RemoveAllTriangles();

	vs->print();
	printf("%u %u\n", kd_child[0]->GetVertexCount(), kd_child[1]->GetVertexCount());

	//Set up the prisms for the two children
	Vector2d min = kd_prism->GetMin();
	Vector2d max = kd_prism->GetMax();

	Vector2d max0 = max;
	Vector2d min1 = min;

	if(kd_splitting_dimension == 0) {
		max0.x = vs->x;
		min1.x = vs->x;
	}
	else {
		max0.y = vs->y;
		min1.y = vs->y;
	}

	Prism* kd_child_prism[2];

	kd_child_prism[0] = new Prism;
	kd_child_prism[0]->SetMin(min);
	kd_child_prism[0]->SetMax(max0);

	kd_child_prism[1] = new Prism;
	kd_child_prism[1]->SetMin(min1);
	kd_child_prism[1]->SetMax(max);

	kd_child[0]->SetKDTreePrisms(kd_tree_prisms);
	kd_child[0]->SetKDPrism(kd_child_prism[0]);

	kd_child[1]->SetKDTreePrisms(kd_tree_prisms);
	kd_child[1]->SetKDPrism(kd_child_prism[1]);

	//Make the children point to this as their parent
	kd_child[0]->SetKDParent(this);
	kd_child[1]->SetKDParent(this);

	//Set up the children with the right splitting dimension
	kd_child[0]->SetKDSplittingDimension((kd_splitting_dimension+1) % 2);
	kd_child[1]->SetKDSplittingDimension((kd_splitting_dimension+1) % 2);

	//Set up the children with a pointer to the leaf nodes
	kd_child[0]->SetKDLeafNodes(kd_leaf_nodes);
	kd_child[1]->SetKDLeafNodes(kd_leaf_nodes);

	if(kd_child[0]->CreateKDTree() == false)
		return false;

	if(kd_child[1]->CreateKDTree() == false)
		return false;

	return true;
}

int TriangleComplex::CombineChildren() {
	//Safety check
	if(kd_child[0] == NULL || kd_child[1] == NULL)
		return false;

	//Make sure the children are leaf nodes
	if(kd_child[0]->IsKDLeafNode() == false || kd_child[1]->IsKDLeafNode() == false)
		return false;

	//Calculate the time spent combining children
	clock_t start_time = clock();

	//Get all the triangles from the children
	for(unsigned int i=0; i<kd_child[0]->GetTriangleCount(); i++) {
		unsigned int tindex = kd_child[0]->GetTriangleIndex(i);
		if(tindex != 0)
			AppendTriangleIndex(tindex);

		//Triangle* tri = kd_child[0]->GetTriangle(i);
		//if(tri == NULL)
		//	continue;

		//AppendTriangle(tri);
	}

	for(unsigned int i=0; i<kd_child[1]->GetTriangleCount(); i++) {
		unsigned int tindex = kd_child[1]->GetTriangleIndex(i);
		if(tindex != 0 && kd_child[1]->IsBridgeTriangleIndex(tindex) == false)
			AppendTriangleIndex(tindex);

		//Triangle* tri = kd_child[1]->GetTriangle(i);
		//if(tri == NULL || kd_child[1]->IsBridgeTriangleIndex(tri->GetLocalIndex()))
		//	continue;

		//AppendTriangle(tri);
	}

	//Get the incomplete vertex lists from the children
	incomplete_vertices.clear();
	incomplete_vertices_angles.clear();

	vector<unsigned int> iv0 = kd_child[0]->GetIncompleteVertices();
	vector<double> iva0 = kd_child[0]->GetIncompleteVerticesAngles();
	//vector<TriangleList> ivat0 = kd_child[0]->GetIncompleteVerticesAdjacentTriangles();

	for(unsigned int i=0; i<iv0.size(); i++) {
		incomplete_vertices.push_back(iv0[i]);
		incomplete_vertices_angles.push_back(iva0[i]);
		//incomplete_vertices_adjacent_triangles.push_back(ivat0[i]);
	}

	vector<unsigned int> iv1 = kd_child[1]->GetIncompleteVertices();
	vector<double> iva1 = kd_child[1]->GetIncompleteVerticesAngles();
	//vector<TriangleList> ivat1 = kd_child[1]->GetIncompleteVerticesAdjacentTriangles();

	for(unsigned int i=0; i<iv1.size(); i++) {
		incomplete_vertices.push_back(iv1[i]);
		incomplete_vertices_angles.push_back(iva1[i]);
		//incomplete_vertices_adjacent_triangles.push_back(ivat1[i]);
	}

	//Remove the children from the kd_leaf_nodes list
	for(unsigned int i=0; i<kd_leaf_nodes->size(); i++) {
		if((*kd_leaf_nodes)[i] == kd_child[0]) {
			kd_leaf_nodes->erase(kd_leaf_nodes->begin() + i);
			break;
		}
	}

	for(unsigned int i=0; i<kd_leaf_nodes->size(); i++) {
		if((*kd_leaf_nodes)[i] == kd_child[1]) {
			kd_leaf_nodes->erase(kd_leaf_nodes->begin() + i);
			break;
		}
	}

	//Erase the data of the children
	if(kd_child[0] != NULL)
		delete kd_child[0];

	if(kd_child[1] != NULL)
		delete kd_child[1];

	//Stop pointing to the children
	kd_child[0] = NULL;
	kd_child[1] = NULL;

	//Set a flag to save some time later
	SetIncompleteListsComputed(true);

	//Calculate the time spent combining children
	clock_t end_time = clock();
	printf("Time spent combining children: %fs\n", double(end_time - start_time) / double(CLOCKS_PER_SEC));

	return true;
}

int TriangleComplex::SetKDParent(TriangleComplex* kd_parent) {
	this->kd_parent = kd_parent;

	return true;
}

int TriangleComplex::SetKDSplittingDimension(int kd_splitting_dimension) {
	this->kd_splitting_dimension = kd_splitting_dimension;

	return true;
}

int TriangleComplex::SetKDPrism(Prism* kd_prism) {
	this->kd_prism = kd_prism;

	if(kd_tree_prisms != NULL)
		kd_tree_prisms->push_back(this->kd_prism);

	return true;
}

int TriangleComplex::SetKDLeafNodes(vector<TriangleComplex*>* kd_leaf_nodes) {
	this->kd_leaf_nodes = kd_leaf_nodes;

	return true;
}

int TriangleComplex::SetKDTreePrisms(PrismList* kd_tree_prisms) {
	this->kd_tree_prisms = kd_tree_prisms;

	return true;
}

TriangleComplex* TriangleComplex::GetKDParent() {
	return kd_parent;
}

int TriangleComplex::IsKDLeafNode() {
	if(kd_child[0] == NULL && kd_child[1] == NULL)
		return true;

	return false;
}

int TriangleComplex::AppendBridgeTriangleIndex(unsigned int local_index) {
	kd_bridge_triangles.push_back(local_index);
	return true;
}

int TriangleComplex::IsBridgeTriangleIndex(unsigned int local_index) {
	for(unsigned int i=0; i<kd_bridge_triangles.size(); i++)
		if(kd_bridge_triangles[i] == local_index)
			return true;

	return false;
}


//Debugging functions
/*int TriangleComplex::write_svg(const char* filename, double w, double h) {
	//Simple error check
	//if(global_vertex_list->size() <= 1)
	//	return false;

	//Open the file for writing
	FILE* handle = fopen(filename, "w");
	if(!handle)
		return false;

	//Write a background rectangle
	fprintf(handle, "<svg viewBox=\"0 0 %f %f\" version=\"1.1\">\n", w, h);
	fprintf(handle, "<rect x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" fill=\"white\"/>\n", 0.0, 0.0, w, h);

	//Write all the triangle/vertex data
	if(write_svg(handle, w, h, true) == false) {
		fclose(handle);
		return false;
	}

	//Close the file and return
	fprintf(handle, "</svg>\n");
	fclose(handle);
	return true;
}*/

/*int TriangleComplex::write_svg(FILE* handle, double w, double h, int draw_verts) {
	//Write the triangles/edges
	//for(unsigned int i=0; i<triangle_list->size(); i++)
	//	(*triangle_list)[i]->write_svg(handle, w, h);
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri != NULL)
			tri->write_svg(handle, w, h);
	}

	//Write the vertices
	if(draw_verts == true) {
		for(unsigned int i=0; i<global_mesh_data->GetVertexCount(); i++) {
		//for(unsigned int i=0; i<global_vertex_list->size(); i++) {
			Vector2d* pt = global_mesh_data->GetVertex(i);
			if(pt == NULL)
				continue;

			//double cx = (*global_vertex_list)[i]->x;
			//double cy = (*global_vertex_list)[i]->y;
			double cx = pt->x;
			double cy = pt->y;
			double r = 1.0;

			fprintf(handle, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"blue\"/>\n", cx, h-cy, r);
		}
	}

	//Write the kd-tree prism
	if(kd_prism != NULL)
		kd_prism->write_svg(handle, w, h);

	//If there are kd-children have them write their own triangle/vertex data
	if(kd_child[0] != NULL) kd_child[0]->write_svg(handle, w, h, false);
	if(kd_child[1] != NULL) kd_child[1]->write_svg(handle, w, h, false);

	return true;
}*/

/*int TriangleComplex::generate_random_vertex_list(int num, double xmin, double xmax) {
	for(int i=0; i<num; i++) {
		Vector2d* new_vertex = new Vector2d;
		new_vertex->set(get_rand(xmin, xmax), get_rand(xmin, xmax));

		unsigned int vindex = global_mesh_data->AppendVertex(new_vertex);
		AppendVertexIndex(vindex);
		//global_vertex_list->push_back(new_vector);
		//AppendVertexIndex(i+1);
	}

	return true;
}*/

//Internal use functions
int TriangleComplex::initialize() {
	vertex_list.clear();
	triangle_list.clear();

	//triangle_list = new TriangleList;
	//triangle_list->clear();

	//Clear some lists
	incomplete_vertices.clear();
	incomplete_vertices_angles.clear();

	incomplete_lists_computed = false;

	//Initialize the kd tree data
	kd_parent = NULL;
	kd_child[0] = NULL;
	kd_child[1] = NULL;

	kd_leaf_nodes = NULL;
	kd_tree_prisms = NULL;

	kd_prism = NULL;
	kd_splitting_dimension = 0;

	kd_bridge_triangles.clear();

	return true;
}

int TriangleComplex::free_data() {
	//Clear the vertex list
	vertex_list.clear();

	//Clear the triangle list
	triangle_list.clear();

	//Delete the triangle list
	/*if(triangle_list) {
		for(unsigned int i=0; i<triangle_list->size(); i++)
			if((*triangle_list)[i] != NULL)
				delete (*triangle_list)[i];

		triangle_list->clear();
		delete triangle_list;
	}*/
	//triangle_list = NULL;

	//Clear some lists
	incomplete_vertices.clear();
	incomplete_vertices_angles.clear();

	//Clean up some kd-tree data
	if(kd_prism != NULL) {
		delete kd_prism;
		kd_prism = NULL;
	}

	//If this is the top node then clear everything
	if(kd_parent == NULL)
		delete kd_tree_prisms;

	kd_bridge_triangles.clear();

	return true;
}

//The most basic triangle mesher
int TriangleComplex::basic_triangle_mesher() {
	//Safety test
	if(GetVertexCount() < 3) {
		printf("Error: Not enough vertices\n");
		return false;
	}

	printf("Starting with %u triangles\n", GetTriangleCount());
	printf("Starting with %u vertices\n", GetVertexCount());

	//If necessary create a seed triangle
	if(create_seed_triangle() == false) {
		printf("Error: Could not create seed triangle\n");
		return false;
	}

	//Figure out the list of incomplete vertices and edges
	if(compute_incomplete_vertices() == false) {
		printf("Error: Could not compute incomplete vertices\n");
		return false;
	}

	time_t start_time = clock();

	//Counts the number of loops
	int count = 0;

	//The meshing algorithm
	int done = false;
	while(!done) {
		//Assert that we are done finding new triangles
		done = true;

		//Search through the list of all the incomplete edges and try to make a new triangle
		//Triangle* new_tri = new Triangle(global_vertex_list);
		Triangle* new_tri = new Triangle(global_mesh_data->GetGlobalVertexList());
		int found_good_triangle = false;

		for(unsigned int i=0; i<GetTriangleCount(); i++) {
			Triangle* tri = GetTriangle(i);
			if(tri == NULL || tri->GetAdjacentTriangleCount() == 3)
				continue;

			for(int opposing_vertex=0; opposing_vertex<3; opposing_vertex++) {
				if(tri->GetAdjacentTriangle(opposing_vertex) != NULL)
					continue;

				for(unsigned int j=0; j<incomplete_vertices.size(); j++) {
					//First make sure this vertex is not degenerate
					Vector2d* vj = GetGlobalVertex(incomplete_vertices[j]);
					if(vj == NULL)
						continue;

					//Next make sure that this vertex is on the correct side of the edge
					if(tri->TestPointEdgeOrientation(opposing_vertex, *vj) != -1)
						continue;

					//Make sure the incomplete vertex is not the opposite vertex for our edge
					if(incomplete_vertices[j] == tri->GetVertexIndex(opposing_vertex))
						continue;

					//Create a test triangle
					new_tri->SetVertex(0, incomplete_vertices[j]);

					if(opposing_vertex == 0) {
						new_tri->SetVertex(1, tri->GetVertexIndex(2));
						new_tri->SetVertex(2, tri->GetVertexIndex(1));
					}
					else if(opposing_vertex == 1) {
						new_tri->SetVertex(1, tri->GetVertexIndex(0));
						new_tri->SetVertex(2, tri->GetVertexIndex(2));
					}
					else if(opposing_vertex == 2) {
						new_tri->SetVertex(1, tri->GetVertexIndex(1));
						new_tri->SetVertex(2, tri->GetVertexIndex(0));
					}

					//Try to orient the vertices, and if its a degenerate triangle skip it
					if(new_tri->OrientVertices() == false)
						continue;

					//Test to see if new_tri overlaps with any vertices
					int found_vertex_overlap = false;
					for(unsigned int k=0; k<GetVertexCount(); k++) {
						if(new_tri->IsVertex(GetVertexIndex(k)) == true)
							continue;

						Vector2d* vk = GetVertex(k);
						if(vk == NULL)
							continue;

						if(new_tri->TestPointInside(*vk, false) == true) {
							found_vertex_overlap = true;
							break;
						}
					}
					if(found_vertex_overlap == true)
						continue;

					//Test to see if new_tri overlaps with any of the other triangles
					int found_triangle_overlap = false;
					for(unsigned int k=0; k<GetTriangleCount(); k++) {
						Triangle* tri = GetTriangle(k);

						if(new_tri->TestOverlap(tri)) {
							found_triangle_overlap = true;
							break;
						}
					}
					if(found_triangle_overlap == true)
						continue;

					//We have found a good triangle
					//if(found_triangle_overlap == false) {
					found_good_triangle = true;
					break;
					//}
				}

				if(found_good_triangle == true)
					break;
			}

			if(found_good_triangle == true)
				break;
		}

		//Add the newly found triangle to the triangle list
		if(found_good_triangle) {
			unsigned int tindex = 0;
			#pragma omp critical
			{
				tindex = AppendTriangle(new_tri);
			}

			//Update triangle adjacencies
			for(unsigned int i=0; i<GetTriangleCount(); i++) {
				Triangle* tri = GetTriangle(i);
				if(tri == NULL || tri->GetAdjacentTriangleCount() == 3)
					continue;

				int opposing_vertex = -1;
				int tri_opposing_vertex = -1;

				if(new_tri->TestAdjacency(tri, opposing_vertex, tri_opposing_vertex) == true) {
					new_tri->SetAdjacentTriangle(opposing_vertex, tri);
					tri->SetAdjacentTriangle(tri_opposing_vertex, new_tri);
				}
			}

			//Update the incomplete vertices
			for(int i=0; i<3; i++) {
				unsigned int vindex = new_tri->GetVertexIndex(i);
				double angle = 0.0;

				if(new_tri->GetVertexAngle(i, angle) == false)
					continue;

				for(unsigned int j=0; j<incomplete_vertices.size(); j++) {
					if(vindex == incomplete_vertices[j]) {
						incomplete_vertices_angles[j] += angle;

						if(incomplete_vertices_angles[j] >= 6.28318) {
							incomplete_vertices.erase(incomplete_vertices.begin() + j);
							incomplete_vertices_angles.erase(incomplete_vertices_angles.begin() + j);
						}

						break;
					}
				}
			}

			//Reset the stop condition
			done = false;
		}

		//Otherwise do some end-of-loop clean-up
		else
			delete new_tri;

		count++;
		//char filename[1000];
		//sprintf(filename, "run%d.svg", count);
		//write_svg(filename, 300, 300);
	}

	printf("incomplete vertices left over: %u\n", incomplete_vertices.size());

	time_t end_time = clock();
	printf("Time spent in basic triangle mesher = %fs\n", double(end_time - start_time) / double(CLOCKS_PER_SEC));

	printf("Triangle count: %u\n", GetTriangleCount());
	return true;
}

int TriangleComplex::create_seed_triangle() {
	//Safety check
	if(GetVertexCount() < 3)
		return false;

	if(GetTriangleCount() == 0) {
		//Sort all the vertices by how close they are to the center
		Vector2d center(0.0, 0.0);

		for(unsigned int i=0; i<GetVertexCount(); i++) {
			Vector2d* v = GetVertex(i);

			if(v != NULL)
				center += ((*v) / double(GetVertexCount()));
		}

		//Quick-sort on vertex_list
		for(unsigned int i=0; i<GetVertexCount(); i++) {
			unsigned int center_closest_vertex = i;

			for(unsigned int j=i+1; j<GetVertexCount(); j++) {
				Vector2d* vc = GetVertex(center_closest_vertex);
				Vector2d* vj = GetVertex(j);

				if(vc == NULL)
					center_closest_vertex = j;

				else if(vj != NULL && vj->distance2(center) < vc->distance2(center))
					center_closest_vertex = j;
			}

			unsigned int vindex = GetVertexIndex(i);
			SetVertexIndex(i, GetVertexIndex(center_closest_vertex));
			SetVertexIndex(center_closest_vertex, vindex);
		}

		//Safety check
		if(GetVertexIndex(0) == 0 || GetVertexIndex(1) == 0)
			return false;

		//Create a triangle
		//Triangle* tri = new Triangle(global_vertex_list);
		Triangle* tri = new Triangle(global_mesh_data->GetGlobalVertexList());

		tri->SetVertex(0, GetVertexIndex(0));
		tri->SetVertex(1, GetVertexIndex(1));

		//Try to complete the triangle
		int found_good_triangle = false;
		for(unsigned int i=2; i<GetVertexCount(); i++) {
			//Try to add a new vertex
			tri->SetVertex(2, GetVertexIndex(i));

			//If the triangle is degenerate reset the vertex to the null vertex
			if(tri->OrientVertices() == false)
				tri->SetVertex(2, 0);

			//Make sure that none of the other vertices are inside this triangle
			else {
				int found_point_inside = false;
				for(unsigned int j=2; j<GetVertexCount(); j++) {
					if(j == i)
						continue;

					Vector2d* pt = GetVertex(j);
					if(pt != NULL && tri->TestPointInside(*pt, false) == true) {
						found_point_inside = true;
						break;
					}
				}

				//This triangle was no good after all, so reset the vertex we tested to null
				if(found_point_inside)
					tri->SetVertex(2, 0);

				//We found a good triangle
				else {
					found_good_triangle = true;
					break;
				}
			}
		}

		//If we found a good triangle
		if(found_good_triangle)
			AppendTriangle(tri);

		//Otherwise if we failed
		else {
			delete tri;
			return false;
		}
	}

	return true;
}

int TriangleComplex::compute_incomplete_vertices() {
	//Safety checking
	if(GetVertexCount() < 3)
		return false;

	//Don't compute anything if the lists have already been computed
	if(incomplete_lists_computed == true)
		return true;

	//Keep track of the time spent in this function
	time_t start_time = clock();

	//Clear the lists
	incomplete_vertices.clear();
	incomplete_vertices_angles.clear();

	//incomplete_vertices_adjacent_triangles.clear();

	//All vertices are considered incomplete for now
	incomplete_vertices = vertex_list;
	incomplete_vertices_angles.resize(GetVertexCount());

	//incomplete_vertices_adjacent_triangles.resize(GetVertexCount());

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);

		//Go through the vertices of this triangle
		for(int j=0; j<3; j++) {
			if(tri->GetVertexIndex(j) != 0) {
				//Manage the incomplete vertices list
				unsigned int vindex = tri->GetVertexIndex(j);

				double angle = 0.0;
				if(tri->GetVertexAngle(j, angle) == false)
					continue;

				for(unsigned int k=0; k<incomplete_vertices.size(); k++) {
					if(incomplete_vertices[k] == vindex) {
						incomplete_vertices_angles[k] += angle;
						//incomplete_vertices_adjacent_triangles[k].push_back(tri);
						break;
					}
				}
			}
		}
	}

	//Screen out the complete vertices from the incomplete ones
	int removed_vertex = false;
	for(unsigned int i=0; i<incomplete_vertices.size(); i++) {
		if(i == 1 && removed_vertex == true) {
			removed_vertex = false;
			i--;
		}

		if(incomplete_vertices_angles[i] >= 6.28318) {
			incomplete_vertices.erase(incomplete_vertices.begin() + i);
			incomplete_vertices_angles.erase(incomplete_vertices_angles.begin() + i);

			removed_vertex = true;
		}

		if(i > 0 && removed_vertex == true) {
			removed_vertex = false;
			i--;
		}
	}

	clock_t end_time = clock();
	printf("Time spent computing incomplete vertices/edges = %fs\n", double(end_time - start_time) / double(CLOCKS_PER_SEC));

	return true;
}

int TriangleComplex::is_vertex_complete(unsigned int vindex, TriangleList adjacent_triangles) {
	//This function assumes that all the triangles are oriented ccw, and
	//basically checks to see if the vertex is completely surrounded by triangles

	//Safety check for the null vertex
	// + returning true means the algorithm will overlook this vertex from now on
	if(vindex == 0)
		return true;

	//A vertex needs to be surrounded by at least three triangles to be complete
	if(adjacent_triangles.size() < 3)
		return false;

	//Try to form a cycle of edges around the vertex
	for(unsigned int i=0; i<adjacent_triangles.size(); i++) {
		unsigned int next_vertex = adjacent_triangles[i]->GetNextVertex(vindex);

		int found = false;
		for(unsigned int j=0; j<adjacent_triangles.size(); j++) {
			//There's no need to check a triangle against itself
			if(i == j)
				continue;

			if(next_vertex == adjacent_triangles[j]->GetPrevVertex(vindex)) {
				found = true;
				break;
			}
		}

		//We've determined that this vertex is not surrounded by triangles
		if(found == false)
			return false;
	}

	//We were able to cycle around the vertex with adjacent triangle edges
	return true;
}

int TriangleComplex::basic_delaunay_flipper() {
	//If there are less than two triangles there are no flips to perform
	if(GetTriangleCount() < 2)
		return true;

	int flip_count = 0;
	int maximum_flip_count = 100;
	for(int iter=0; iter<maximum_flip_count; iter++) {
		//Assert that there is no delaunay flip performed this run
		int flip_performed = false;
		//printf("iter: %d\n", iter);

		for(unsigned int i=0; i<GetTriangleCount(); i++) {
			Triangle* tri = GetTriangle(i);

			//Skip bridge triangles for flipping
			if(tri == NULL || IsBridgeTriangleIndex(tri->GetLocalIndex()) == true)
				continue;

			//Go through each adjacent triangle of tri
			for(int k=0; k<3; k++) {
				Triangle* adj_tri = tri->GetAdjacentTriangle(k);

				//Skip bridge triangles for flipping
				if(adj_tri == NULL || IsBridgeTriangleIndex(adj_tri->GetLocalIndex()) == true)
					continue;
				
				//Perform a flip if the delaunay condition fails
				if(tri->TestDelaunay(k) == false) {
					tri->PerformDelaunayFlip(k);
					flip_performed = true;
				}
			}
		}

		//Stop if we've converged to a delaunay triangulation
		if(flip_performed == false)
			break;
	}

	return true;
}

//The most basic stretched grid method
int TriangleComplex::basic_stretched_grid_method(unsigned int iterations, double alpha) {
	//These are used by the SGM algorithm and are pre-calculated here
	vector<unsigned int> vertex_triangle_count;
	vector<bool> vertex_clamped;
	vector<double> vertex_angles;

	double average_edge_length = 0.0;
	double total_edge_count = 0.0;

	/*vertex_triangle_count.resize(global_vertex_list->size(), 0);
	vertex_clamped.resize(global_vertex_list->size(), true);
	vertex_angles.resize(global_vertex_list->size(), 0.0);*/
	vertex_triangle_count.resize(global_mesh_data->GetVertexCount(), 0);
	vertex_clamped.resize(global_mesh_data->GetVertexCount(), true);
	vertex_angles.resize(global_mesh_data->GetVertexCount(), 0.0);

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri == NULL)
			continue;

		for(int j=0; j<3; j++) {
			unsigned int vj_index = tri->GetVertexIndex(j);

			double vj_angle = 0.0;
			if(tri->GetVertexAngle(j, vj_angle) == true) {
				vertex_triangle_count[vj_index]++;
				vertex_angles[vj_index] += vj_angle;
			}
		}
	}

	for(unsigned int i=0; i<GetVertexCount(); i++) {
		unsigned int vindex = GetVertexIndex(i);

		if(vertex_angles[vindex] < 6.28318)
			continue;

		if(vertex_triangle_count[vindex] == 0)
			continue;

		vertex_clamped[vindex] = false;
	}

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri == NULL)
			continue;

		for(int j=0; j<3; j++) {
			unsigned int vj_index = tri->GetVertexIndex(j);

			Vector2d* vj = tri->GetVertex(j);
			if(vj == NULL)
				continue;

			Vector2d* v1 = NULL;
			Vector2d* v2 = NULL;

			Triangle* adj1 = NULL;
			Triangle* adj2 = NULL;

			if(tri->GetAdjacentVerticesTriangles(j, v1, v2, adj1, adj2) != false) {
				if(adj2 == NULL) {
					average_edge_length += vj->distance(*v1) * 0.5;
					total_edge_count += 0.5;
				}
				else {
					average_edge_length += vj->distance(*v1) * 0.25;
					total_edge_count += 0.25;
				}

				if(adj1 == NULL) {
					average_edge_length += vj->distance(*v2) * 0.5;
					total_edge_count += 0.5;
				}
				else {
					average_edge_length += vj->distance(*v2) * 0.25;
					total_edge_count += 0.25;
				}
			}
		}
	}
	average_edge_length /= total_edge_count;
	printf("Averaged edge length: %f\n", average_edge_length);
	printf("Total edge count: %f\n", total_edge_count);

	//Run the stretched grid method
	for(int iter=0; iter<iterations; iter++) {
		vector<Vector2d> vertex_adjustments;
		vertex_adjustments.resize(global_mesh_data->GetVertexCount(), Vector2d(0, 0));

		for(unsigned int i=0; i<GetTriangleCount(); i++) {
			Triangle* tri = GetTriangle(i);
			if(tri == NULL)
				continue;

			for(int j=0; j<3; j++) {
				unsigned int vj_index = tri->GetVertexIndex(j);

				Vector2d* vj = tri->GetVertex(j);
				if(vj == NULL)
					continue;

				if(vertex_clamped[vj_index] == false) {
					Vector2d* v1 = NULL;
					Vector2d* v2 = NULL;

					Triangle* adj1 = NULL;
					Triangle* adj2 = NULL;
					if(tri->GetAdjacentVerticesTriangles(j, v1, v2, adj1, adj2) != false) {
						//vertex_adjustments[vj_index] += (*v1 - *vj)*0.5;
						//vertex_adjustments[vj_index] += (*v2 - *vj)*0.5;

						/*double v1_dist = vj->distance(*v1);
						double v2_dist = vj->distance(*v2);

						double v1_factor = (v1_dist - average_edge_length) / v1_dist;
						double v2_factor = (v2_dist - average_edge_length) / v2_dist;*/

						if(adj2 == NULL)
							vertex_adjustments[vj_index] += (*v1 - *vj) * 0.5;

						else
							vertex_adjustments[vj_index] += (*v1 - *vj) * 0.25;

						if(adj1 == NULL)
							vertex_adjustments[vj_index] += (*v2 - *vj) * 0.5;

						else
							vertex_adjustments[vj_index] += (*v2 - *vj) * 0.25;


						/*double dist2 = vj->distance2(*v1);
						if(dist2 < alpha) dist2 = alpha;
						vertex_adjustments[vj_index] += (*vj - *v1)*0.5/dist2;

						dist2 = vj->distance2(*v2);
						if(dist2 < alpha) dist2 = alpha;
						vertex_adjustments[vj_index] += (*vj - *v2)*0.5/dist2;*/
					}
				}
			}
		}

		/*#pragma omp parallel for
		for(unsigned int i=0; i<global_vertex_list->size(); i++) {
			Vector2d* vi = GetGlobalVertex(i);
			if(vi == NULL)
				continue;

			for(unsigned int j=i+1; j<global_vertex_list->size(); j++) {
				Vector2d* vj = GetGlobalVertex(j);
				if(vj == NULL)
					continue;

				double dist2 = vi->distance2(*vj);
				if(dist2 < alpha) dist2 = alpha;

				vertex_adjustments[i] -= (*vi - *vj)/dist2;
			}
		}*/

		for(unsigned int i=0; i<global_mesh_data->GetVertexCount(); i++) {
			if(vertex_clamped[i] == false) {
				Vector2d* v = GetGlobalVertex(i);
				if(v == NULL)
					continue;

				//vertex_adjustments[i] /= double(vertex_triangle_count[i]);
				(*v) = (*v) + (vertex_adjustments[i] * (alpha/vertex_triangle_count[i]));
			}
		}
	}

	printf("DONE WITH STRETCHED GRID!\n");
	return true;
}

//The force based stretched grid method
int TriangleComplex::force_stretched_grid_method(int iterations, double dt) {
	//First collect all the edge information
	vector<Edge*> edge_list;
	double average_edge_length = 0.0;

	if(GetEdges(edge_list) == false)
		return false;

	if(ComputeAverageEdgeLength(average_edge_length, edge_list) == false)
		return false;

	//Next create some data that will be used by the algorithm
	vector<int> use_vertex;
	vector<double> vertex_angle;
	vector<unsigned int> vertex_triangle_count;

	vector<Vector2d> vertex_velocity;
	vector<Vector2d> vertex_force;

	//unsigned int iterations = 100;
	//double dt = 0.01;

	//Resize some lists
	use_vertex.resize(global_mesh_data->GetVertexCount(), false);
	vertex_angle.resize(global_mesh_data->GetVertexCount(), 0.0);
	vertex_triangle_count.resize(global_mesh_data->GetVertexCount(), 0);

	vertex_velocity.resize(global_mesh_data->GetVertexCount(), Vector2d(0,0));
	vertex_force.resize(global_mesh_data->GetVertexCount(), Vector2d(0,0));

	//Do some calculations to see which vertices to actually use
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri == NULL)
			continue;

		for(int j=0; j<3; j++) {
			unsigned int vj_index = tri->GetVertexIndex(j);

			double vj_angle = 0.0;
			if(tri->GetVertexAngle(j, vj_angle) == true) {
				vertex_triangle_count[vj_index]++;
				vertex_angle[vj_index] += vj_angle;
			}
		}
	}

	for(unsigned int i=0; i<GetVertexCount(); i++) {
		unsigned int vindex = GetVertexIndex(i);

		if(vertex_angle[vindex] < 6.28318)
			continue;

		if(vertex_triangle_count[vindex] == 0)
			continue;

		use_vertex[vindex] = true;
	}

	//Run the actual stretched grid algorithm
	for(unsigned int iter=0; iter<iterations; iter++) {
		//Reset the vertex forces
		for(unsigned int i=0; i<vertex_force.size(); i++) {
			vertex_force[i].x = 0.0;
			vertex_force[i].y = 0.0;
		}

		//Calculate the force on each vertex with the edge list
		for(unsigned int i=0; i<edge_list.size(); i++) {
			unsigned int v0_index = edge_list[i]->GetVertexIndex(0);
			unsigned int v1_index = edge_list[i]->GetVertexIndex(1);

			Vector2d* v0 = edge_list[i]->GetVertex(0);
			Vector2d* v1 = edge_list[i]->GetVertex(1);

			if(v0 == NULL || v1 == NULL)
				continue;

			double length = edge_list[i]->ComputeLength();
			Vector2d rhat = (*v1 - *v0).normalize();

			double dl = (length - average_edge_length);

			vertex_force[v0_index] += (rhat * dl);
			vertex_force[v1_index] -= (rhat * dl);
		}

		//Update velocity and position
		for(unsigned int i=0; i<GetVertexCount(); i++) {
			unsigned int vindex = GetVertexIndex(i);
			Vector2d* pt = GetVertex(i);

			if(use_vertex[vindex] == true && pt != NULL) {
				vertex_velocity[vindex] += (vertex_force[vindex] * dt);
				*pt = *pt + (vertex_velocity[vindex] * dt);
			}
		}
	}


	printf("DONE WITH STRETCHED GRID!\n");
	return true;
}

int TriangleComplex::basic_mesh_cleaner() {
	vector<unsigned int> triangles_to_delete;

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri == NULL)
			continue;

		for(int j=0; j<3; j++) {
			double angle = 0.0;
			if(tri->GetVertexAngle(j, angle) != false && angle > 3.141592/2.0) {
				if(tri->GetAdjacentTriangleCount() < 3) {
					triangles_to_delete.push_back(i);
					break;
				}
			}
		}
	}

	printf("Number of triangles to be deleted: %u\n", triangles_to_delete.size());
	for(unsigned int i=triangles_to_delete.size(); i>0; i--)
		DeleteTriangle(triangles_to_delete[i-1]);

	return true;
}

//This function refines a mesh that is not dense enough
int TriangleComplex::refine_mesh(double desired_edge_length, unsigned int& edge_count, double& average_edge_length) {
	//Split up obtuse edges
	if(split_obtuse_edges(desired_edge_length, edge_count, average_edge_length) == false)
		return false;

	return true;
}

//Splits up edges that are across from an obtuse angle in a triangle
// + this function stops if the desired cell edge length is achieved,
//   or if there are no more edges across from obtuse angles to subdivide
int TriangleComplex::split_obtuse_edges(double desired_edge_length, unsigned int& edge_count, double& average_edge_length) {
	printf("Starting with edge count: %u; average edge length: %f\n", edge_count, average_edge_length);

	int split_triangle = false;
	vector<Triangle*> final_new_triangles;
	vector<unsigned int> final_new_vindices;

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		if(split_triangle == true) {
			split_triangle = false;
			i = 0;
		}

		Triangle* tri = GetTriangle(i);
		if(tri == NULL)
			continue;

		//Look at each edge of the triangle
		for(int j=0; j<3; j++) {
			double angle = 0.0;
			if(tri->GetVertexAngle(j, angle) == false)
				continue;

			//If this is an acute/right angle then skip it
			if(angle <= PI/2.0 + EFF_ZERO)
				continue;

			printf("Found obtuse angle\n");

			double edge_length = tri->ComputeEdgeLength(j);

			int split_count = 0;
			if(desired_edge_length < EFF_ZERO)
				split_count = 1;
			else
				split_count = max(1, int(ceil(edge_length / desired_edge_length)));

			printf("split count: %d; edge_length: %f; desired edge length: %f\n", split_count, edge_length, desired_edge_length);

			vector<Triangle*> new_triangles;
			vector<unsigned int> new_vindices;
			double average_new_edge_length = 0.0;

			if(tri->SubdivideAlongEdge(j, split_count, new_triangles, new_vindices, average_new_edge_length) == true) {
				Triangle* adj_tri = tri->GetAdjacentTriangle(j);

				printf("Deleting some old triangles\n");
				if(adj_tri != NULL) {
					unsigned int adj_tindex;
					for(unsigned int tindex = 0; tindex<GetTriangleCount(); tindex++) {
						if(GetTriangle(tindex) == adj_tri) {
							adj_tindex = tindex;
							break;
						}
					}

					printf("adj_tindex: %u\n", adj_tindex);
					if(adj_tindex > i) {
						DeleteTriangle(adj_tindex);
						DeleteTriangle(i);
					}
					else {
						DeleteTriangle(i);
						DeleteTriangle(adj_tindex);
					}
				}
				else
					DeleteTriangle(i);
				printf("Deletion successful\n");

				//Append the new vertex indices and triangles to the final lists
				for(unsigned int k=0; k<new_vindices.size(); k++)
					AppendVertexIndex(new_vindices[k]);
					//final_new_vindices.push_back(new_vindices[k]);

				for(unsigned int k=0; k<new_triangles.size(); k++)
					AppendTriangle(new_triangles[k]);
					//final_new_triangles.push_back(new_triangles[k]);

				printf("ASDF\n");
				split_triangle = true;

				//Update the average cell edge length
				int new_edge_count = 2*split_count + 1;
				if(adj_tri != NULL)
					new_edge_count += split_count;

				printf("ASDF2\n");
				double avg_buf1 = average_edge_length * (double(edge_count) / double(edge_count + new_edge_count - 1));
				double avg_buf2 = average_new_edge_length * (double(new_edge_count) / double(edge_count + new_edge_count - 1));
				printf("average edge length: %f; edge count: %u\n", average_edge_length, edge_count);
				printf("new edge count: %u; average new edge length: %f\n", new_edge_count, average_new_edge_length);
				printf("avg_buf1: %f; avg_buf2: %f\n", avg_buf1, avg_buf2);
				average_edge_length = avg_buf1 + avg_buf2;
				edge_count += new_edge_count - 1;
				printf("ASDF3\n");
			}
			else
				return false;

			printf("Subdivision done; edge count: %u; average length: %f\n", edge_count, average_edge_length);

			if(split_triangle == true)
				break;
		}

		//Quit if we have refined below the desired cell edge length
		if(average_edge_length <= desired_edge_length)
			break;

		if(split_triangle == true && i == GetTriangleCount()-1)
			i--;
	}

	/*for(unsigned int i=0; i<final_new_vindices.size(); i++)
		AppendVertexIndex(final_new_vindices[i]);

	for(unsigned int i=0; i<final_new_triangles.size(); i++)
		AppendTriangle(final_new_triangles[i]);*/

	return true;
}

//Barycentric subdivide triangles to achieve a desired edge length
int TriangleComplex::barycentric_subdivion(double desired_cell_edge_length, double& average_edge_length) {

	return true;
}

//Initialize the kd-tree prism based on the vertex list
int TriangleComplex::compute_kd_prism() {
	//Safety check
	if(GetVertexCount() == 0)
		return false;

	if(kd_prism != NULL)
		delete kd_prism;

	kd_prism = new Prism;


	int prism_initialized = false;
	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* vi = GetVertex(i);

		if(vi != NULL) {
			if(prism_initialized == false) {
				kd_prism->SetMinMax(*vi, *vi);
				prism_initialized = true;
			}

			else
				kd_prism->Expand(*vi);
		}
	}

	return true;
}

int TriangleComplex::sort_vertices_by_coordinate(int dim) {
	//Run a quick sort on the vertex list according to x/y coordinate
	for(unsigned int i=0; i<GetVertexCount(); i++) {
		unsigned int minimal_index = i;
		Vector2d* vm = GetVertex(i);

		for(unsigned int j=i+1; j<GetVertexCount(); j++) {
			Vector2d* vj = GetVertex(j);

			if(vm == NULL) {
				vm = vj;
				minimal_index = j;
			}

			else if(vj != NULL) {
				//Sort by x coordinate
				if(dim == 0 && vm->x > vj->x) {
					vm = vj;
					minimal_index = j;
				}

				//Sort by y coordinate
				else if(dim == 1 && vm->y > vj->y) {
					vm = vj;
					minimal_index = j;
				}
			}
		}

		//Switch the i'th vertex with the minimal_index'th vertex
		unsigned int vbuf = GetVertexIndex(i);
		SetVertexIndex(i, GetVertexIndex(minimal_index));
		SetVertexIndex(minimal_index, vbuf);
	}

	return true;
}

unsigned int TriangleComplex::compute_centermost_vertex(int dim) {
	//Compute the average value of all the vertex components in the direction dim
	double avg = 0.0;
	double total_vertex_count = double(GetVertexCount());

	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* vi = GetVertex(i);

		if(vi != NULL) {
			if(dim == 0)
				avg += vi->x / total_vertex_count;

			else if(dim == 1)
				avg += vi->y / total_vertex_count;
		}
	}

	//Figure out which vertex is closest to the avg in the direction dim
	Vector2d* vc = GetVertex(0);
	unsigned int closest_index = 0;

	for(unsigned int i=0; i<GetVertexCount(); i++) {
		if(vc == NULL) {
			closest_index = i;
			vc = GetVertex(i);
		}

		else {
			Vector2d* vi = GetVertex(i);

			if(vi != NULL) {
				if(dim == 0 && fabs(vi->x - avg) < fabs(vc->x - avg)) {
					vc = vi;
					closest_index = i;
				}

				else if(dim == 1 && fabs(vi->y - avg) < fabs(vc->y - avg)) {
					vc = vi;
					closest_index = i;
				}
			}
		}
	}

	return closest_index;
}

int TriangleComplex::compute_triangle_adjacencies() {
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);

		if(tri == NULL)
			continue;

		for(int j=0; j<3; j++) {
			if(tri->GetAdjacentTriangleCount() == 3)
				continue;

			for(int k=0; k<GetTriangleCount(); k++) {
				if(k == i)
					continue;

				Triangle* adj_tri = GetTriangle(k);
				if(adj_tri == NULL || adj_tri->GetAdjacentTriangleCount() == 3)
					continue;

				int opposing_vertex = -1;
				int adj_opposing_vertex = -1;

				if(tri->TestAdjacency(adj_tri, opposing_vertex, adj_opposing_vertex) == true) {
					tri->SetAdjacentTriangle(opposing_vertex, adj_tri);
					adj_tri->SetAdjacentTriangle(adj_opposing_vertex, tri);
				}

				if(tri->GetAdjacentTriangleCount() == 3)
					break;
			}
		}
	}

	return true;
}
