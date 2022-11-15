#pragma once
#include<vector>
#include<string>
#include"Mesh.h"

class Shader;
class Mesh;
struct Halfedge;
struct Face;
struct Vertex;

using GLuint = unsigned int;

class Model {
public: 
	Model();
	~Model();
	void render(const Shader& shader);
	void loadFromFile(const std::string& path);
	void fromMesh(const Mesh& mesh);
	void collapseEdge(int index);
	void randomCollapse();
public:
	std::vector<Halfedge*> halfedges;
	std::vector<Face*> faces;
	std::vector<Vertex*> vertices;

	bool dirty;
	int validFaces; 
	int validEdges;
	int validVertices;
	GLuint VAO, VBO, EBO;

};
