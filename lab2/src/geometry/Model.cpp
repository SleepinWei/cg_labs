#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Model.h"
#include"half-edge.h"
#include"../utils/Shader.h"
#include"Mesh.h"
#include<random>

template <class T>
static int statical_cnt(std::vector<T> statical_arr) {
	int res = 0;
	for (const auto& element : statical_arr) {
		if (element->valid)
			++res;
	}
	return res;
}

void Model::randomCollapse() {
	//while (1) {

	//	Halfedge* edge = halfedges[1215];
	//	std::cout << edge->id << " " << edge->next->id << " " << edge->next->next->id << " " << edge->next->next->next->id << std::endl;

	//	Halfedge* edge_tmp1 = edge->opposite;
	//	std::cout << edge_tmp1->id << " " << edge_tmp1->next->id << " " << edge_tmp1->next->next->id << " " << edge_tmp1->next->next->next->id << std::endl;
	//}

	//int cnt = 0;
	//for (const auto halfedge : halfedges) {
	//	if (!halfedge->opposite) {
	//		++cnt;
	//		std::cout << halfedge->id << std::endl;
	//	}
	//}
	//std::cout << vertices.size() << " " << cnt << std::endl;
	

	int r = 0;
	while (r < halfedges.size()) {
		if (halfedges[r]->valid)
			break;
		r++;
	}
	//std::cout << r << std::endl;
	collapseEdge(r);

	//while (true) {
	//	int r = 0;
	//	while (r < halfedges.size()) {
	//		if (halfedges[r]->valid)
	//			break;
	//		r++;
	//	}
	//	std::cout << r << std::endl;
	//	collapseEdge(r);
	//}
	//collapseEdge(1000);
}
void Model::collapseEdge(int index) {
	int tmp_validedges = validEdges, tmp_validfaces = validFaces, tmp_validvertex = validVertices;
	bool hasoppsite = false;
	//
	Halfedge* edge = halfedges[index];

	edge->valid = false;

	Vertex* v0 = edge->next->next->incident_vertex;
	Vertex* v1 = edge->incident_vertex;

	Face* f0 = edge->incident_face;

	Halfedge* next = edge->next;
	next->valid = false;
	Halfedge* behind = edge->next->next;
	behind->valid = false;
	f0->valid = false;
	f0->halfedge = nullptr;

	v0->position = (v0->position + v1->position) / 2.0f;
	v1->valid = false;

	if(behind->opposite)
		behind->opposite->opposite = next->opposite;
	if(next->opposite)
		next->opposite->opposite = behind->opposite;
	hasoppsite = true;

	// opposite
	if (edge->opposite) {
		edge->opposite->valid = false;
		Face* f1 = edge->opposite->incident_face;
		f1->valid = false;
		f1->halfedge = nullptr;
		Halfedge* onext = edge->opposite->next;
		onext->valid = false;
		Halfedge* obehind = edge->opposite->next->next;
		obehind->valid = false;

		if(onext->opposite)
			onext->opposite->opposite = obehind->opposite;
		if(obehind->opposite)
			obehind->opposite->opposite = onext->opposite;
	}

	// finally, the incident halfedge
	//正向遍历
	Halfedge* beg = edge->next->opposite;
	if (beg) {
		Halfedge* iter = beg;
		iter->incident_vertex = v0;
		iter = iter->next->opposite;
		while (iter && beg != iter) {
			iter->incident_vertex = v0;
			iter = iter->next->opposite;
		}
	}
	

	//逆向遍历
	beg = edge->next->next->opposite;
	if (beg) {
		Halfedge* iter = beg->next->next;	
		iter->incident_vertex = v0;
		iter = iter->opposite;
		while (iter && beg != iter) {
			iter = iter->next->next;
			iter->incident_vertex = v0;
			iter = iter->opposite;
		}
	}

	// delete edge

	// NO release of memory
	dirty = true;

	validFaces = statical_cnt<Face*>(faces);
	validVertices = statical_cnt<Vertex*>(vertices);
	validEdges = statical_cnt<Halfedge*>(halfedges);

	if (tmp_validfaces - validFaces != 2 || tmp_validvertex - validVertices != 1 || tmp_validedges - validEdges != 6) {
		std::cout << "tmpFaces: " << tmp_validfaces << " tmpVertices: " << tmp_validvertex << " tmpEdges: " << tmp_validedges << std::endl;
		std::cout << "validFaces: " << validFaces << " validVertices: " << validVertices << " validEdges: " << validEdges << std::endl;
		std::cout << index << std::endl;
	}
	
}


void Model::fromMesh(const Mesh& mesh) {
	//TODO: construct 'Model' from 'Mesh'
	auto& mvertices = mesh.vertices;
	auto& mindices = mesh.indices;
	std::vector<std::vector<Halfedge*>> map(mvertices.size(), std::vector<Halfedge*>(mvertices.size(), nullptr));
	for (int i = 0; i < mvertices.size(); i++) {
		Vertex* v = new Vertex;
		v->position = mvertices[i]; 
		this->vertices.push_back(v);
	}

	for (int i = 0; i < mindices.size() / 3; i++) {
		int index0 = mindices[3 * i];
		int index1 = mindices[3 * i + 1];
		int index2 = mindices[3 * i + 2];
		Vertex* v0 = this->vertices[index0];
		Vertex* v1 = this->vertices[index1];
		Vertex* v2 = this->vertices[index2];

		Halfedge* edge[3];
		for (int i = 0; i < 3;i++) 
			edge[i] = new Halfedge;
		map[index0][index1] = edge[1];
		map[index1][index2] = edge[2];
		map[index2][index0] = edge[0];

		Face* face = new Face;

		// face
		face->halfedge = edge[0];

		// edge
		edge[0]->next = edge[1]; 
		edge[1]->next = edge[2];
		edge[2]->next = edge[0];
		edge[0]->incident_face = face;
		edge[1]->incident_face = face;
		edge[2]->incident_face = face;
		edge[0]->incident_vertex = v0;
		edge[1]->incident_vertex = v1;
		edge[2]->incident_vertex = v2;

		//opposite
		if (map[index0][index2]) {
			edge[0]->opposite = map[index0][index2];
			edge[0]->opposite->opposite = edge[0];
		}
		if (map[index1][index0]) {
			edge[1]->opposite = map[index1][index0];
			edge[1]->opposite->opposite = edge[1];
		}
		if (map[index2][index1]) {
			edge[2]->opposite = map[index2][index1];
			edge[2]->opposite->opposite = edge[2];
		}

		// vertex
		if(v0->indcident_halfedge == nullptr)
			v0->indcident_halfedge = edge[0];
		if(v1->indcident_halfedge == nullptr)
			v1->indcident_halfedge = edge[1];
		if(v2->indcident_halfedge == nullptr)
			v2->indcident_halfedge = edge[2];

		for (int i = 0; i < 3; i++) {
			this->halfedges.push_back(edge[i]);
		}
		this->faces.push_back(face);
	}

	//for (int i = 0; i < mindices.size() / 3; ++i) {
	//	int index0 = mindices[3 * i];
	//	int index1 = mindices[3 * i + 1];
	//	int index2 = mindices[3 * i + 2];
	//	//opposite
	//	if (map[index0][index2] && map[index2][index0]) {
	//		map[index0][index2]->opposite = map[index2][index0];
	//		map[index2][index0]->opposite = map[index0][index2];
	//	}
	//	if (map[index1][index0] && map[index0][index1]) {
	//		map[index0][index1]->opposite = map[index1][index0];
	//		map[index1][index0]->opposite = map[index0][index1];
	//	}
	//	if (map[index2][index1] && map[index1][index2]) {
	//		map[index2][index1]->opposite = map[index1][index2];
	//		map[index1][index2]->opposite = map[index2][index1];
	//	}
	//}

	validFaces = this->faces.size();
	validEdges = this->halfedges.size();
	validVertices = this->vertices.size();
}

void Model::loadFromFile(const std::string& path) {
	Mesh mesh = Mesh::loadMesh(path);
	fromMesh(mesh);
}

Model::Model() {
	VAO = VBO = EBO = 0;
	dirty = true;
	validFaces = 0;
	validVertices = 0;
	validEdges = 0;
}

Model::~Model() {
	if (VAO) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		
		for (auto p : halfedges) {
			if(p != nullptr)
			delete p;
		}
		for (auto face : faces) {
			if(face !=nullptr)
			delete face;
		}
		for (auto v : vertices) {
			if(v != nullptr)
			delete v;
		}
	}
}

void Model::render(const Shader& shader) {
	if (VAO == 0) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	if (dirty) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, faces.size() * 9 * sizeof(float), nullptr, GL_STATIC_DRAW);
		for (int i = 0; i < faces.size(); i++) {
			if (faces[i]->valid) {
				// only register valid faces;
				Halfedge* current = faces[i]->halfedge;
				if(!current->valid)
					std::cout << "current halfedge whether valid: " << current->valid << std::endl;
				for (int j = 0; j < 3; j++) {
					Vertex* v = current->incident_vertex;
					if(!v->valid)
						std::cout << "current incident_vertex whether valid: " << v->valid << std::endl;
					glBufferSubData(GL_ARRAY_BUFFER, (9 * i + 3 * j) * sizeof(float), 3 * sizeof(float), &(v->position));
					current = current->next;
				}
			}
		}
		dirty = false;
	}

	// rendering
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, validFaces * 3);
}

