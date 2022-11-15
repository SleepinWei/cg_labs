#include"Mesh.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<iostream>

void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
Mesh processMesh(aiMesh* mesh, const aiScene* scene);

Mesh::Mesh() {
	;
}
Mesh::Mesh(const std::vector<glm::vec3>& vertices_, const std::vector<unsigned int>& indices_):
	vertices(vertices_),indices(indices_)
{
	;
}

Mesh Mesh::loadShape(Shape shape, int n) {
    Mesh mesh;
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            mesh.vertices.push_back({2 * (j * 1.0f/n) -1,0.0f,2*(i*1.0f/n)});
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int index = i * (n+1) + j;
            mesh.indices.push_back(index);
            mesh.indices.push_back(index + n + 1);
            mesh.indices.push_back(index + 1);

            mesh.indices.push_back(index + 1);
            mesh.indices.push_back(index + n + 1);
            mesh.indices.push_back(index + n + 2);
        }
    }
    return mesh;
}

Mesh combineMesh(const std::vector<Mesh>& meshes) {
    // TODO:
    Mesh resultMesh;
    int beginIndex = 0;
	auto& resultVertices = resultMesh.vertices;
	auto& resultIndices = resultMesh.indices;
    for (auto& submesh : meshes) {
        auto& subVertices = submesh.vertices;
        auto& subIndices = submesh.indices;

        resultVertices.insert(resultVertices.end(), subVertices.begin(), subVertices.end());
        // indices append
        for (auto index : subIndices) {
            resultIndices.push_back(index + beginIndex);
        }
        beginIndex += subVertices.size();
    }
    return resultMesh;
}

Mesh Mesh::loadMesh(std::string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,aiProcess_JoinIdenticalVertices);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << '\n';
        return Mesh();
    }
    // retrieve the directory path of the filepath
    //std::string directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    std::vector<Mesh> meshes;
    processNode(scene->mRootNode, scene, meshes);

    return combineMesh(meshes);
}


// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene,meshes);
    }

}

Mesh processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector <glm::vec3> vertices;
    std::vector<unsigned int> indices;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 vertex; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vertex.x = mesh->mVertices[i].x;
        vertex.y = mesh->mVertices[i].y;
        vertex.z = mesh->mVertices[i].z;

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices);
}

