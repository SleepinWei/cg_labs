#pragma once
#include<glm/glm.hpp>

struct Halfedge;
struct Face {
	int id;
	Halfedge* halfedge;

	Face() {
		id = 0;
		halfedge = nullptr;
	}
};

struct Vertex {
	int id;
	glm::vec3 position;
	Halfedge* indcident_halfedge;

	Vertex() {
		id = 0;
		position = glm::vec3(0.0f);
		indcident_halfedge = nullptr;
	}
};

struct Halfedge{
	int id;
	Halfedge* next;
	Halfedge* opposite;
	Face* incident_face;
	Vertex* incident_vertex;

	Halfedge() {
		id = 0;
		next = nullptr;
		opposite = nullptr;
		incident_face = nullptr;
		incident_vertex = nullptr;
	}
};
