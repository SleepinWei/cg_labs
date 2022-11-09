#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Model.h"
#include"half-edge.h"
#include"../utils/Shader.h"
#include"Mesh.h"
#include<random>

void Model::randomCollapse() {
	int r = 0;
	while (r < halfedges.size()) {
		if (halfedges[r]->valid)
			break;
		r++;
	}

	collapseEdge(r);
}
void Model::collapseEdge(int index) {
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

	v0->position = (v0->position + v1->position) / 2.0f;
	v1->valid = false;

	behind->opposite->opposite = next->opposite;
	next->opposite->opposite = behind->opposite;
	hasoppsite = true;

	// opposite
	edge->opposite->valid = false;
	Face* f1 = edge->opposite->incident_face;
	f1->valid = false;
	Halfedge* onext = edge->opposite->next;
	onext->valid = false;
	Halfedge* obehind = edge->opposite->next->next;
	obehind->valid = false;

	onext->opposite->opposite = obehind->opposite;
	obehind->opposite->opposite = onext->opposite;

	// finally, the incident halfedge
	Halfedge* beg = edge->next->opposite;
	Halfedge* iter = beg;
	iter->incident_vertex = v0; 
	iter = iter->next->opposite;
	while (beg != iter) {
		iter->incident_vertex = v0;
		iter = iter->next->opposite;
	}
	// delete edge

	// NO release of memory
	dirty = true;
	validFaces -= 2;
	validVertices -= 1;
	validEdges -= 6;
}

void Model::fromMesh(const Mesh& mesh) {
	//TODO: construct 'Model' from 'Mesh'
	auto& mvertices = mesh.vertices;
	auto& mindices = mesh.indices;
	std::vector<std::vector<Halfedge*>> map(mvertices.size(), std::vector<Halfedge*>(mvertices.size(), nullptr));
	for (int i = 0; i < mvertices.size(); i++) {
		Vertex* v = new Vertex;
		v->position = mvertices[i]; 
		this->vertices.emplace_back(v);
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
				for (int j = 0; j < 3; j++) {
					Vertex* v = current->incident_vertex;
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

