#include "global_mesh_data.h"

GlobalMeshData::GlobalMeshData() {
	global_vertex_list.clear();
	global_triangle_list.clear();

	global_vertex_list.push_back(NULL);
	global_triangle_list.push_back(NULL);
}

GlobalMeshData::~GlobalMeshData() {
	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* v = GetVertex(i);
		if(v != NULL)
			delete v;
	}

	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri != NULL)
			delete tri;
	}
}

//////////////
// File i/o //
//////////////

int GlobalMeshData::LoadFromFile(const char* filename, int load_triangles) {
	//Open the mesh file as an XML document
	XML_Document* xml_document = new XML_Document;
	if(xml_document->LoadFromFile(filename) == false) {
		printf("Error loading the mesh file\n");

		delete xml_document;
		return false;
	}

	if(LoadFromFile(xml_document, load_triangles) == false) {
		delete xml_document;
		return false;
	}

	delete xml_document;
	return true;
}

int GlobalMeshData::LoadFromFile(XML_Document* xml_document, int load_triangles) {
	//Free all data
	FreeAllData();

	//Load vertices
	if(load_vertices_from_file(xml_document) == false)
		return false;

	if(load_triangles == true) {
		//Load triangles
		if(load_triangles_from_file(xml_document) == false)
			return false;
	}

	return true;
}

int GlobalMeshData::SaveToFile(const char* filename, int save_triangles) {
	//Open the output file as an xml document
	XML_Document* xml_document = new XML_Document;

	if(SaveToFile(xml_document, save_triangles) == false) {
		delete xml_document;
		return false;
	}

	if(xml_document->WriteToFile(filename) == false) {
		delete xml_document;
		return false;
	}

	return true;
}

int GlobalMeshData::SaveToFile(XML_Document* xml_document, int save_triangles) {
	XML_TreeNode* head_node = xml_document->GetHeadNode();

	if(GetVertexCount() > 1) {
		//Create a vertexlist
		XML_TreeNode* vertexlist_node = head_node->CreateChildTagPair("vertexlist");

		//Write the vertices to the file
		for(unsigned int i=0; i<GetVertexCount(); i++) {
			Vector2d* pt = GetVertex(i);

			if(pt == NULL)
				continue;

			XML_TreeNode* vertex_node = vertexlist_node->CreateChildTagOpenClosed("vertex");

			XML_Tag* vertex_node_tag = vertex_node->GetStartTag();
			vertex_node_tag->AppendTagAttribute("index", i);
			vertex_node_tag->AppendTagAttribute("x", pt->x);
			vertex_node_tag->AppendTagAttribute("y", pt->y);
		}
	}

	if(save_triangles == true && GetTriangleCount() > 1) {
		//Create a trianglelist
		XML_TreeNode* trianglelist_node = head_node->CreateChildTagPair("trianglelist");

		//Write the triangles to the file
		for(unsigned int i=0; i<GetTriangleCount(); i++) {
			Triangle* tri = GetTriangle(i);

			if(tri == NULL)
				continue;

			XML_TreeNode* triangle_node = trianglelist_node->CreateChildTagOpenClosed("triangle");
			XML_Tag* triangle_node_tag = triangle_node->GetStartTag();

			triangle_node_tag->AppendTagAttribute("index", tri->GetTriangleIndex());

			triangle_node_tag->AppendTagAttribute("n0", tri->GetVertexIndex(0));
			triangle_node_tag->AppendTagAttribute("n1", tri->GetVertexIndex(1));
			triangle_node_tag->AppendTagAttribute("n2", tri->GetVertexIndex(2));

			for(int j=0; j<3; j++) {
				char buffer[1000];
				sprintf(buffer, "a%d", j);

				if(tri->GetAdjacentTriangle(j) == NULL)
					triangle_node_tag->AppendTagAttribute(string(buffer), 0);

				else {
					unsigned int adj_local_index = tri->GetAdjacentTriangle(j)->GetTriangleIndex();
					triangle_node_tag->AppendTagAttribute(string(buffer), adj_local_index);
				}
			}
		}
	}

	return true;
}

int GlobalMeshData::WriteSVG(const char* filename, double width, double height) {
	//Used to format some output
	char buffer[1000];

	XML_Document* xml_document = new XML_Document;
	XML_TreeNode* head_node = xml_document->GetHeadNode();

	printf("ASDF1\n");
	//Create the svg tag
	XML_TreeNode* svg_head_node = head_node->CreateChildTagPair("svg");
	XML_Tag* svg_head_node_tag = svg_head_node->GetStartTag();

	sprintf(buffer, "0 0 %f %f", width, height);
	svg_head_node_tag->AppendTagAttribute("viewBox", buffer);
	svg_head_node_tag->AppendTagAttribute("version", "1.1");

	printf("ASDF2\n");
	//Create a rectangle
	XML_TreeNode* svg_rect = svg_head_node->CreateChildTagOpenClosed("rect");
	XML_Tag* svg_rect_tag = svg_rect->GetStartTag();

	svg_rect_tag->AppendTagAttribute("width", width);
	svg_rect_tag->AppendTagAttribute("height", height);
	svg_rect_tag->AppendTagAttribute("fill", "white");

	printf("ASDF3\n");
	//Draw the triangles
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri == NULL)
			continue;

		Vector2d* v0 = tri->GetVertex(0);
		Vector2d* v1 = tri->GetVertex(1);
		Vector2d* v2 = tri->GetVertex(2);

		if(v0 == NULL || v1 == NULL || v2 == NULL)
			continue;

		XML_TreeNode* svg_polygon = svg_head_node->CreateChildTagOpenClosed("polygon");
		XML_Tag* svg_polygon_tag = svg_polygon->GetStartTag();

		sprintf(buffer, "%f,%f %f,%f %f,%f", v0->x, height-v0->y, v1->x, height-v1->y, v2->x, height-v2->y);
		svg_polygon_tag->AppendTagAttribute("points", buffer);
		svg_polygon_tag->AppendTagAttribute("fill", "green");
		svg_polygon_tag->AppendTagAttribute("stroke", "black");
		svg_polygon_tag->AppendTagAttribute("stroke-width", "2");
		svg_polygon_tag->AppendTagAttribute("style", "fill-opacity:0.5");
	}

	printf("ASDF4\n");
	//Draw the vertices
	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* pt = GetVertex(i);
		if(pt == NULL)
			continue;

		XML_TreeNode* svg_circle = svg_head_node->CreateChildTagOpenClosed("circle");
		XML_Tag* svg_circle_tag = svg_circle->GetStartTag();

		svg_circle_tag->AppendTagAttribute("cx", pt->x);
		svg_circle_tag->AppendTagAttribute("cy", height - pt->y);
		svg_circle_tag->AppendTagAttribute("r", 1.0);
		svg_circle_tag->AppendTagAttribute("fill", "blue");
	}

	printf("ASDF5\n");
	//Write the svg file
	if(xml_document->WriteToFile(filename) == false) {
		delete xml_document;
		return false;
	}

	printf("ASDF6\n");
	delete xml_document;
	return true;
}

///////////////////////////////
// Data managament functions //
///////////////////////////////

//Free data
int GlobalMeshData::FreeAllData() {
	if(FreeTriangleData() == false)
		return false;

	if(FreeVertexData() == false)
		return false;

	return true;
}

int GlobalMeshData::FreeVertexData() {
	for(unsigned int i=0; i<GetVertexCount(); i++) {
		Vector2d* v = GetVertex(i);
		if(v != NULL)
			delete v;
	}
	global_vertex_list.clear();
	global_vertex_list.push_back(NULL);

	return true;
}

int GlobalMeshData::FreeTriangleData() {
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri = GetTriangle(i);
		if(tri != NULL)
			delete tri;
	}
	global_triangle_list.clear();
	global_triangle_list.push_back(NULL);

	return true;
}

//Vertices
unsigned int GlobalMeshData::GetVertexCount() {
	return global_vertex_list.size();
}

Vector2d* GlobalMeshData::GetVertex(unsigned int index) {
	if(index >= GetVertexCount())
		return NULL;

	return global_vertex_list[index];
}

int GlobalMeshData::SetVertex(unsigned int index, Vector2d* vertex) {
	if(index >= GetVertexCount())
		global_vertex_list.resize(index+1, NULL);

	global_vertex_list[index] = vertex;
	return true;
}

unsigned int GlobalMeshData::AppendVertex(Vector2d* vertex) {
	global_vertex_list.push_back(vertex);
	return (global_vertex_list.size()-1);
}

VertexList* GlobalMeshData::GetGlobalVertexList() {
	return &global_vertex_list;
}

//Triangles
unsigned int GlobalMeshData::GetTriangleCount() {
	return global_triangle_list.size();
}

Triangle* GlobalMeshData::GetTriangle(unsigned int index) {
	if(index >= GetTriangleCount())
		return NULL;

	return global_triangle_list[index];
}

int GlobalMeshData::SetTriangle(unsigned int index, Triangle* tri) {
	if(index >= GetTriangleCount())
		global_triangle_list.resize(index+1, NULL);

	global_triangle_list[index] = tri;
	tri->SetTriangleIndex(index);
	return true;
}

unsigned int GlobalMeshData::AppendTriangle(Triangle* tri) {
	global_triangle_list.push_back(tri);
	tri->SetTriangleIndex(global_triangle_list.size()-1);

	return (global_triangle_list.size()-1);
}

int GlobalMeshData::DeleteTriangle(unsigned int tindex) {
	Triangle* tri = GetTriangle(tindex);
	if(tri == NULL)
		return true;

	int adj_count = tri->GetAdjacentTriangleCount();
	int count = 0;

	//Remove the triangle from any adjacencies
	for(unsigned int i=0; i<GetTriangleCount(); i++) {
		Triangle* tri2 = GetTriangle(i);
		if(tri2 == NULL)
			continue;

		for(int j=0; j<3; j++) {
			if(tri2->GetAdjacentTriangle(j) == tri) {
				tri2->SetAdjacentTriangle(j, NULL);
				count++;
				break;
			}
		}

		if(count == adj_count)
			break;
	}

	//Delete the actual triangle
	delete global_triangle_list[tindex];
	global_triangle_list[tindex] = NULL;

	return true;
}

TriangleList* GlobalMeshData::GetGlobalTriangleList() {
	return &global_triangle_list;
}

////////////////////////////
// Internal use functions //
////////////////////////////

int GlobalMeshData::load_vertices_from_file(XML_Document* xml_document) {
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
			if(index_str == "")
				return false;

			unsigned int index = (unsigned int) atoi(index_str.c_str());

			//The 0 index is reserved
			if(index == 0)
				return false;

			string vx_str = new_vertex_tag->GetAttributeValue("x");
			if(vx_str == "")
				return false;

			string vy_str = new_vertex_tag->GetAttributeValue("y");
			if(vy_str == "")
				return false;

			double vx = atof(vx_str.c_str());
			double vy = atof(vy_str.c_str());

			Vector2d* pt = GetVertex(index);
			if(pt != NULL) {
				pt->x = vx;
				pt->y = vy;
			}
			else {
				Vector2d* new_vertex = new Vector2d;
				new_vertex->x = vx;
				new_vertex->y = vy;
				SetVertex(index, new_vertex);
			}
		}
	}

	return true;
}

int GlobalMeshData::load_triangles_from_file(XML_Document* xml_document) {
	//Load in the triangles from the mesh file
	vector<XML_TreeNode*> trianglelists;
	xml_document->GetHeadNode()->GetTreeNodesOfTagName("trianglelist", trianglelists);

	for(unsigned int i=0; i<trianglelists.size(); i++) {
		XML_TreeNode* trianglelist = trianglelists[i];

		vector<XML_Tag*> new_triangle_tags;
		trianglelist->GetTagsOfTagName("triangle", new_triangle_tags);

		for(unsigned int j=0; j<new_triangle_tags.size(); j++) {
			XML_Tag* triangle_tag = new_triangle_tags[j];

			string index_str = triangle_tag->GetAttributeValue("index");
			if(index_str == "")
				return false;

			unsigned int index = (unsigned int) atoi(index_str.c_str());

			//The 0 index is reserved
			if(index == 0)
				return false;

			unsigned int n0 = 0;
			unsigned int n1 = 0;
			unsigned int n2 = 0;

			string n0_str = triangle_tag->GetAttributeValue("n0");
			if(n0_str != "") n0 = (unsigned int) atoi(n0_str.c_str());

			string n1_str = triangle_tag->GetAttributeValue("n1");
			if(n1_str != "") n1 = (unsigned int) atoi(n1_str.c_str());

			string n2_str = triangle_tag->GetAttributeValue("n2");
			if(n2_str != "") n2 = (unsigned int) atoi(n2_str.c_str());

			if(n0 == 0 || n1 == 0 || n2 == 0)
				return false;

			Triangle* tri = GetTriangle(index);
			if(tri != NULL) {
				tri->SetVertex(0, n0);
				tri->SetVertex(1, n1);
				tri->SetVertex(2, n2);
			}
			else {
				//If no triangle with this index existed create a new one
				Triangle* new_tri = new Triangle(GetGlobalVertexList());

				new_tri->SetTriangleIndex(index);

				new_tri->SetVertex(0, n0);
				new_tri->SetVertex(1, n1);
				new_tri->SetVertex(2, n2);

				SetTriangle(index, new_tri);
			}
		}
	}

	//Load in the triangle adjacency information
	for(unsigned int i=0; i<trianglelists.size(); i++) {
		XML_TreeNode* trianglelist = trianglelists[i];

		vector<XML_Tag*> new_triangle_tags;
		trianglelist->GetTagsOfTagName("triangle", new_triangle_tags);

		for(unsigned int j=0; j<new_triangle_tags.size(); j++) {
			XML_Tag* triangle_tag = new_triangle_tags[j];

			string index_str = triangle_tag->GetAttributeValue("index");
			if(index_str == "")
				return false;

			unsigned int index = (unsigned int) atoi(index_str.c_str());

			//The 0 index is reserved
			if(index == 0)
				return false;

			unsigned int a0 = 0;
			unsigned int a1 = 0;
			unsigned int a2 = 0;

			string a0_str = triangle_tag->GetAttributeValue("a0");
			if(a0_str != "") a0 = (unsigned int) atoi(a0_str.c_str());

			string a1_str = triangle_tag->GetAttributeValue("a1");
			if(a1_str != "") a1 = (unsigned int) atoi(a1_str.c_str());

			string a2_str = triangle_tag->GetAttributeValue("a2");
			if(a2_str != "") a2 = (unsigned int) atoi(a2_str.c_str());

			Triangle* tri = GetTriangle(index);
			if(tri == NULL)
				return false;

			Triangle* adj_tri0 = NULL;
			Triangle* adj_tri1 = NULL;
			Triangle* adj_tri2 = NULL;

			adj_tri0 = GetTriangle(a0);
			adj_tri1 = GetTriangle(a1);
			adj_tri2 = GetTriangle(a2);

			tri->SetAdjacentTriangle(0, adj_tri0);
			tri->SetAdjacentTriangle(1, adj_tri1);
			tri->SetAdjacentTriangle(2, adj_tri2);
		}
	}

	return true;
}
