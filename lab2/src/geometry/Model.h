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
public:
	std::vector<Halfedge*> halfedges;
	std::vector<Face*> faces;
	std::vector<Vertex*> vertices;
private:
	void fromMesh(const Mesh& mesh);

	GLuint VAO, VBO, EBO;
};
