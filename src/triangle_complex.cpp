#include "triangle_complex.h"

TriangleComplex::TriangleComplex() {
	//Create the global vertex list from scratch
	// + The 0th vertex is always a NULL vertex
	//global_vertex_list = new VertexList;
	//global_vertex_list->push_back(NULL);

	//Create the global vertex list
	global_vertex_list = NULL;
	reset_global_vertex_list();

	//Initialize all the other variables
	initialize();
}

TriangleComplex::TriangleComplex(VertexList* global_vertex_list) {
	//Get the global vertex list from the function input
	this->global_vertex_list = global_vertex_list;

	//Initialize all the other variables
	initialize();
}

TriangleComplex::~TriangleComplex() {
	free_data();
}

//General use functions
int TriangleComplex::LoadFromFile(const char* filename) {
	//First free up all the old data
	free_data();
	initialize();

	//Reset the global vertex list
	reset_global_vertex_list();

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

			global_vertex_list->push_back(new_vertex);
			AppendVertexIndex(global_vertex_list->size()-1);
		}
	}

	//Load in the edges from the mesh file

	//Load in the triangles from the mesh file
	vector<XML_TreeNode*> trianglelists;
	xml_document->GetHeadNode()->GetTreeNodesOfTagName("trianglelist", trianglelists);

	for(unsigned int i=0; i<trianglelists.size(); i++) {
		XML_TreeNode* trianglelist = trianglelists[i];

		vector<XML_Tag*> new_triangle_tags;
		trianglelist->GetTagsOfTagName("triangle", new_triangle_tags);

		for(unsigned int j=0; j<new_triangle_tags.size(); j++) {
		}
	}

	delete xml_document;
	return true;
}

int TriangleComplex::WriteToFile(const char* filename) {
	//Some simple error checking
	if(global_vertex_list->size() <= 1)
		return false;

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

	//Open the file for writing
	/*FILE* handle = fopen(filename, "w");
	if(!handle)
		return false;

	//Write the vertex list to a file
	fprintf(handle, "<vertexlist>\n");

	for(unsigned int i=0; i<global_vertex_list->size(); i++) {
		//Skip the first vertex, it is always null
		if(i == 0)
			continue;

		double x = (*global_vertex_list)[i]->x;
		double y = (*global_vertex_list)[i]->y;

		fprintf(handle, "\t<vertex index=\"%u\" x=\"%f\" y=\"%f\"/>\n", i, x, y);
	}
	fprintf(handle, "</vertexlist>\n\n");

	//Write the triangle list to a file
	if(WriteToFile(handle) == false) {
		fclose(handle);
		return false;
	}

	//Close up and return
	fclose(handle);
	return true;*/
}

/*int TriangleComplex::WriteToFile(FILE* handle) {
	if(triangle_list->size() == 0)
		return true;

	fprintf(handle, "<trianglelist>\n");

	for(unsigned int i=0; i<triangle_list->size(); i++) {
		unsigned int n1 = (*triangle_list)[i]->GetVertexIndex(0);
		unsigned int n2 = (*triangle_list)[i]->GetVertexIndex(1);
		unsigned int n3 = (*triangle_list)[i]->GetVertexIndex(2);

		fprintf(handle, "\t<triangle n1=\"%u\" n2=\"%u\" n3=\"%u\"/>\n", n1, n2, n3);
	}

	fprintf(handle, "</trianglelist>\n\n");

	return true;
}*/

//Data management functions
unsigned int TriangleComplex::GetTriangleCount() {
	return triangle_list->size();
}

Triangle* TriangleComplex::GetTriangle(unsigned int tindex) {
	return (*triangle_list)[tindex];
}

int TriangleComplex::RemoveTriangle(unsigned int tindex) {
	//Safety check
	if(tindex >= triangle_list->size())
		return false;

	Triangle* tri = GetTriangle(tindex);

	if(tri != NULL) {
		//Remove the triangle from any adjacencies
		for(unsigned int i=0; i<triangle_list->size(); i++) {
			for(int j=0; j<3; j++) {
				if((*triangle_list)[i]->GetAdjacentTriangle(j) == (*triangle_list)[tindex])
					(*triangle_list)[i]->SetAdjacentTriangle(j, NULL);
			}
		}

		//Delete the actual triangle
		delete (*triangle_list)[tindex];
	}

	//Finally erase the triangle from the triangle list
	triangle_list->erase(triangle_list->begin() + tindex);

	return true;
}

unsigned int TriangleComplex::AppendTriangle(Triangle* tri) {
	unsigned int tindex = triangle_list->size();
	triangle_list->push_back(tri);

	return tindex;
}

unsigned int TriangleComplex::GetVertexCount() {
	return vertex_list->size();
}

unsigned int TriangleComplex::GetVertexIndex(unsigned int vertex) {
	//Safety check
	if(vertex >= GetVertexCount())
		return 0;

	return (*vertex_list)[vertex];
}

Vector2d* TriangleComplex::GetVertex(unsigned int vertex) {
	//Safety check
	if(vertex >= GetVertexCount())
		return NULL;

	unsigned int vindex = GetVertexIndex(vertex);
	return (*global_vertex_list)[vindex];
}

Vector2d* TriangleComplex::GetGlobalVertex(unsigned int vindex) {
	//Safety check
	if(vindex >= global_vertex_list->size())
		return NULL;

	return (*global_vertex_list)[vindex];
}

int TriangleComplex::SetVertexIndex(unsigned int vertex, unsigned int vindex) {
	//Safety check
	if(vertex >= GetVertexCount())
		return false;

	(*vertex_list)[vertex] = vindex;
	return true;
}

int TriangleComplex::AppendVertexIndex(unsigned int vindex) {
	vertex_list->push_back(vindex);

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
	if(kd_parent == NULL && global_vertex_list->size() >= MAXIMUM_MESH_SIZE) {
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
					new_kd_leaf_nodes.push_back((*kd_leaf_nodes)[0]);
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
	}

	//Otherwise
	else {
		if(basic_triangle_mesher() == false)
			return false;

		if(basic_delaunay_flipper() == false)
			return false;
	}

	return true;
}

int TriangleComplex::RunDelaunayFlips() {
	/*if(GetVertexCount() > MAXIMUM_MESH_SIZE) {
		//Split up mesh via kd-tree
	}

	else {
		//Run a simple triangle mesher
		if(basic_delaunay_flipper() == false)
			return false;
	}*/

	if(basic_delaunay_flipper() == false)
		return false;

	return true;
}


//K-d tree related functions
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
	kd_child[0] = new TriangleComplex(global_vertex_list);
	kd_child[1] = new TriangleComplex(global_vertex_list);

	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* vi = GetVertex(i);

		if(vi != NULL) {
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
	}

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

	//Calculate the time spent combining children
	clock_t start_time = clock();

	//Get all the triangles from the children
	for(unsigned int i=0; i<kd_child[0]->GetTriangleCount(); i++)
		AppendTriangle(kd_child[0]->GetTriangle(i));

	for(unsigned int i=0; i<kd_child[1]->GetTriangleCount(); i++)
		AppendTriangle(kd_child[1]->GetTriangle(i));

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


//Debugging functions
int TriangleComplex::write_svg(const char* filename, double w, double h) {
	//Simple error check
	if(global_vertex_list->size() <= 1)
		return false;

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
}

int TriangleComplex::write_svg(FILE* handle, double w, double h, int draw_verts) {
	//Write the triangles/edges
	for(unsigned int i=0; i<triangle_list->size(); i++)
		(*triangle_list)[i]->write_svg(handle, w, h);

	//Write the vertices
	if(draw_verts == true) {
		for(unsigned int i=0; i<global_vertex_list->size(); i++) {
			//Skip the first vertex, it is null
			if(i == 0)
				continue;

			double cx = (*global_vertex_list)[i]->x;
			double cy = (*global_vertex_list)[i]->y;
			double r = 1.0;

			fprintf(handle, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"blue\"/>\n", cx, h-cy, r);
		}
	}

	//Write the incomplete edges
	/*for(unsigned int i=0; i<incomplete_edges.size(); i++) {
		TriangleEdge te = incomplete_edges[i];

		Vector2d* p0 = GetGlobalVertex(te.vertices[0]);
		Vector2d* p1 = GetGlobalVertex(te.vertices[1]);

		if(p0 == NULL || p1 == NULL)
			continue;

		fprintf(handle, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" ", p0->x, h-p0->y, p1->x, h-p1->y);
		fprintf(handle, "stroke=\"purple\" stroke-width=\"3\"/>\n");
	}

	//Write the incomplete vertices
	for(unsigned int i=0; i<incomplete_vertices.size(); i++) {
		//Skip the first vertex, it is null
		if(i == 0)
			continue;

		Vector2d* pt = GetGlobalVertex(incomplete_vertices[i]);
		if(pt == NULL)
			continue;

		double cx = pt->x;
		double cy = pt->y;
		double r = 3.0;

		fprintf(handle, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" fill=\"red\"/>\n", cx, h-cy, r);
	}*/

	//Write the kd-tree prism
	if(kd_prism != NULL)
		kd_prism->write_svg(handle, w, h);

	//If there are kd-children have them write their own triangle/vertex data
	if(kd_child[0] != NULL) kd_child[0]->write_svg(handle, w, h, false);
	if(kd_child[1] != NULL) kd_child[1]->write_svg(handle, w, h, false);

	return true;
}

int TriangleComplex::generate_random_vertex_list(int num, double xmin, double xmax) {
	for(int i=0; i<num; i++) {
		Vector2d* new_vector = new Vector2d;
		new_vector->set(get_rand(xmin, xmax), get_rand(xmin, xmax));

		global_vertex_list->push_back(new_vector);
		AppendVertexIndex(i+1);
	}

	return true;
}


//Internal use functions
int TriangleComplex::initialize() {
	vertex_list = new vector<unsigned int>;
	vertex_list->clear();

	triangle_list = new TriangleList;
	triangle_list->clear();

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

	return true;
}

int TriangleComplex::free_data() {
	//Delete the vertex list
	vertex_list->clear();
	delete vertex_list;
	vertex_list = NULL;

	//Delete the triangle list
	/*if(triangle_list) {
		for(unsigned int i=0; i<triangle_list->size(); i++)
			if((*triangle_list)[i] != NULL)
				delete (*triangle_list)[i];

		triangle_list->clear();
		delete triangle_list;
	}*/
	triangle_list = NULL;

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

	return true;
}

int TriangleComplex::reset_global_vertex_list() {
	if(global_vertex_list != NULL) {
		for(unsigned int i=0; i<global_vertex_list->size(); i++) {
			if((*global_vertex_list)[i] != NULL)
				delete (*global_vertex_list)[i];
		}
		delete global_vertex_list;
	}

	global_vertex_list = new VertexList;
	global_vertex_list->push_back(NULL);

	return true;
}

int TriangleComplex::load_vertices(FILE* handle) {
	//Read in starting from a <vertexlist> tag
	char buffer[4096];
	fgets(buffer, 4096, handle);

	while(!feof(handle) && strstr(buffer, "</vertexlist>") == NULL) {
		Vector2d* new_vertex = NULL;
		if(load_vertex_tag(buffer, new_vertex) == true) {
			global_vertex_list->push_back(new_vertex);
			AppendVertexIndex(global_vertex_list->size()-1);
		}

		//vertexlist_block += buffer;
		fgets(buffer, 4096, handle);
	}

	return true;
}

int TriangleComplex::load_triangles(FILE* handle) {
	//Read in starting from a <trianglelist> tag
	
	return true;
}

int TriangleComplex::load_vertex_tag(char* str, Vector2d* &res) {
	//Initialize res to null for now
	res = NULL;

	char* p = strstr(str, "<vertex ");
	if(p == NULL)
		return false;

	double vx = 0.0;
	double vy = 0.0;

	//Load in the x coordinate
	char* px = strstr(p, " x");
	if(px == NULL)
		return false;

	while(*px && !isAlphaNumeric(*px, true)) px++;
	if(px == NULL)
		return false;

	vx = atof(px);

	//Load in the y coordinate
	char* py = strstr(p, " y");
	if(py == NULL)
		return false;

	while(*py && !isAlphaNumeric(*py, true)) py++;
	if(py == NULL)
		return false;

	vy = atof(py);

	//We were able to load in all the necessary data so create a new vector
	res = new Vector2d(vx, vy);
	return true;
}

int TriangleComplex::load_triangle_tag(char* str, Triangle* &res) {

	return true;
}

int TriangleComplex::basic_triangle_mesher() {
	//Safety test
	if(GetVertexCount() < 3) {
		printf("Error: Not enough vertices\n");
		return false;
	}

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
		Triangle* new_tri = new Triangle(global_vertex_list);
		int found_good_triangle = false;

		for(unsigned int i=0; i<GetTriangleCount(); i++) {
			Triangle* tri = GetTriangle(i);
			if(tri == NULL || tri->GetAdjacentTriangleCount() == 3)
				continue;

			/*int opposing_vertex = -1;

			for(int j=0; j<3; j++) {
				if(tri->GetAdjacentTriangle(j) == NULL) {
					opposing_vertex = j;
					break;
				}
			}

			if(opposing_vertex == -1)
				continue;*/

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
					if(incomplete_vertices[j] == tri->GetVertexIndex(opposing_vertex)) {
						//printf("OPPOSITE VERTEX FAIL\n");
						continue;
					}

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
						Vector2d* vk = GetVertex(k);
						if(vk == NULL)
							continue;

						if(new_tri->TestPointInside(*vk, true) == true) {
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

					//We have found a good triangle
					if(found_triangle_overlap == false) {
						found_good_triangle = true;
						break;
					}
				}
			}

			if(found_good_triangle == true)
				break;
		}

		//Add the newly found triangle to the triangle list
		if(found_good_triangle) {
			unsigned int tindex = AppendTriangle(new_tri);

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
		Triangle* tri = new Triangle(global_vertex_list);

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
	incomplete_vertices = *vertex_list;
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

			//Skip outer triangles for now
			//if(tri->GetAdjacentTriangleCount() < 3)
			//	continue;

			//Go through each adjacent triangle of tri
			for(int k=0; k<3; k++) {
				//if(tri->GetAdjacentTriangle(k) != NULL && tri->GetAdjacentTriangle(k)->GetAdjacentTriangleCount() < 3)
				//	continue;
				
				//Perform a flip if the delaunay condition fails
				if(tri->TestDelaunay(k) == false) {
					tri->PerformDelaunayFlip(k);
					flip_performed = true;
					//printf("PERFORMED A FLIP\n");
					//char filename[1000];
					//sprintf(filename, "run%d.svg", flip_count++);
					//write_svg(filename, 1000, 1000);
				}
			}
		}

		//Stop if we've converged to a perfect triangulation
		if(flip_performed == false)
			break;
	}

	return true;
}

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
