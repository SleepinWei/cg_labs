#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
enum class Shape {
    PLANE
};
class Mesh {
public:
    // mesh Data
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    // constructor
    Mesh();
    Mesh(const std::vector<glm::vec3>& vertices_, const std::vector<unsigned int>& indices_);

	static Mesh loadMesh(std::string const& path);
    static Mesh loadShape(Shape shape, int n);
};
