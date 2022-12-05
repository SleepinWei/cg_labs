#pragma once
#include<vector>
#include<string>
#include"Mesh.h"
#include<queue>

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
	struct cmp {
		bool operator()(int x, int y) {
			return x > y;
		};
	};

	std::vector<Halfedge*> halfedges;
	std::vector<Face*> faces;
	std::vector<Vertex*> vertices;
	std::priority_queue<int*, std::vector<int*>, cmp> queue;


	bool dirty;
	int validFaces; 
	int validEdges;
	int validVertices;
	GLuint VAO, VBO, EBO;

};
