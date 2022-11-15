#pragma once
#include<glm/glm.hpp>

int face_idcnt = 0;
int v_idcnt = 0;
int edge_idcnt = 0;
struct Halfedge;
struct Face {
	int id;
	Halfedge* halfedge;
	bool valid;

	Face() {
		id = ++face_idcnt;
		valid = true;
		halfedge = nullptr;
	}
};

struct Vertex {
	int id;
	glm::vec3 position;
	Halfedge* indcident_halfedge;

	bool valid;

	Vertex() {
		id = ++v_idcnt;
		position = glm::vec3(0.0f);
		indcident_halfedge = nullptr;
		valid = true;
	}
};

struct Halfedge{
	int id;
	Halfedge* next;
	Halfedge* opposite;
	Face* incident_face;
	Vertex* incident_vertex;

	bool valid;

	Halfedge() {
		id = ++edge_idcnt;
		next = nullptr;
		opposite = nullptr;
		incident_face = nullptr;
		incident_vertex = nullptr;
		valid = true;
	}
};
