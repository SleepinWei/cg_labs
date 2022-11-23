#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Model.h"
#include"half-edge.h"
#include"../utils/Shader.h"
#include"Mesh.h"
#include<random>

template <class T>
static int statistic_cnt(std::vector<T> statical_arr) {
	int res = 0;
	for (const auto& element : statical_arr) {
		if (element->valid)
			++res;
	}
	return res;
}

void Model::randomCollapse() {
	int order_for_idx = 0;
	//TODO: design a algorithm to fill the appropriate value into order_for_idx to call the function collapseEdge
	// you are supposed to ensure that each call to this function can reduce the number of face
	


	//TODO -- END

	collapseEdge(order_for_idx);
	validFaces = statistic_cnt<Face*>(faces);
	validVertices = statistic_cnt<Vertex*>(vertices);
	validEdges = statistic_cnt<Halfedge*>(halfedges);
}
void Model::collapseEdge(int index) {
	//TODO: design a algorithm to collapse the face(edges) with the halfedge[index]


	//TODO -- END
}


void Model::fromMesh(const Mesh& mesh) {
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

	//TODO: you are supposed to fill in the opposite attribute


	//TODO -- END

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

	int validCounter = 0;

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
					glBufferSubData(GL_ARRAY_BUFFER, (9 * validCounter + 3 * j) * sizeof(float), 3 * sizeof(float), &(v->position));
					current = current->next;
				}

				++validCounter;
			}
		}
		dirty = false;
	}

	// rendering
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, validFaces * 3);
}

