#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Model.h"
#include"half-edge.h"
#include"../utils/Shader.h"
#include"Mesh.h"

void Model::fromMesh(const Mesh& mesh) {
	//TODO: construct 'Model' from 'Mesh'
	auto& mvertices = mesh.vertices;
	auto& mindices = mesh.indices;
	for (int i = 0; i < mvertices.size(); i++) {
		Vertex* v = new Vertex;
		v->position = mvertices[i]; 
		this->vertices.emplace_back(v);
	}

	for (int i = 0; i < mindices.size() / 3; i++) {
		Vertex* v0 = this->vertices[mindices[3 * i]];
		Vertex* v1 = this->vertices[mindices[3 * i + 1]];
		Vertex* v2 = this->vertices[mindices[3 * i + 2]];

		Halfedge* edge[3];
		for (int i = 0; i < 3;i++) 
			edge[i] = new Halfedge[3];
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
		if (v2->indcident_halfedge)
			edge[0]->opposite = v2->indcident_halfedge;
		if (v1->indcident_halfedge) {
			edge[2]->opposite = v1->indcident_halfedge;
		}
		if (v0->indcident_halfedge) {
			edge[1]->opposite = v0->indcident_halfedge;
		}

		// vertex
		v0->indcident_halfedge = edge[0];
		v1->indcident_halfedge = edge[1];
		v2->indcident_halfedge = edge[2];

		for (int i = 0; i < 3; i++) {
			this->halfedges.push_back(edge[i]);
		}
		this->faces.push_back(face);
	}
}

void Model::loadFromFile(const std::string& path) {
	Mesh mesh = Mesh::loadMesh(path);
	fromMesh(mesh);
}

Model::Model() {
	VAO = VBO = EBO = 0;
}

Model::~Model() {
	if (VAO) {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		
		for (auto p : halfedges) {
			delete p;
		}
		for (auto face : faces) {
			delete face;
		}
		for (auto v : vertices) {
			delete v;
		}
	}
}

void Model::render(const Shader& shader) {
	if (VAO == 0) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//TODO;
        glBufferData(GL_ARRAY_BUFFER, faces.size() * 9 * sizeof(float), nullptr , GL_STATIC_DRAW);
		for (int i = 0; i < faces.size(); i++) {
			Halfedge* current = faces[i]->halfedge;
			for (int j = 0; j < 3; j++) {
				Vertex* v = current->incident_vertex;
				glBufferSubData(GL_ARRAY_BUFFER, (9 * i + 3*j) * sizeof(float), 3 * sizeof(float), &(v->position));
				current = current->next;
			}
		}
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int),(void*)mesh.indices.data(),GL_STATIC_DRAW);
	}

	glBindVertexArray(VAO);

    //glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT,0);
	glDrawArrays(GL_TRIANGLES, 0, faces.size() * 3);
}

