//
// Created by fpeterek on 02.11.20.
//

#ifndef ZPG_PROJEKT_MODEL_HPP
#define ZPG_PROJEKT_MODEL_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "enums.hpp"
#include "shader.hpp"
#include "texture.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};


struct Material {
    glm::vec3 diffuse { 0.f };
    glm::vec3 specular { 0.f };
    glm::vec3 ambient { 0.f };
    std::string diffuseMap;
    std::string specularMap;
    std::string heightMap;
    float shininess = 0.f;
};

class Mesh {

    GLuint vbo = 0;
    GLuint vao = 0;
    GLuint ebo = 0;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::shared_ptr<Texture> texture;

    const Material & material;

    void init();

public:

    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Texture> texture, const Material & material);
    Mesh(Mesh && mesh) noexcept;

    void bindAndDraw(unsigned int id, Shader & shader) const;

    void applyTexture(std::shared_ptr<Texture> texture);

};

class ModelLoader;

class Model {

    friend ModelLoader;

    void addMesh(Mesh && mesh);

    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    std::string directory;

public:

    void draw(uint32_t id, Shader & shader) const;
    void applyTexture(const std::shared_ptr<Texture> & texture);

};

class ModelLoader {

    static std::unordered_map<std::string, std::shared_ptr<Model>> models;

    static std::shared_ptr<Model> loadModel(const std::string & model);

    static constexpr uint32_t importOpts = aiProcess_OptimizeMeshes
            bitor aiProcess_JoinIdenticalVertices
            bitor aiProcess_Triangulate
            bitor aiProcess_CalcTangentSpace
            // bitor aiProcess_FlipUVs
            bitor aiProcess_GenSmoothNormals;

    static std::string formatName(const std::string & model);

    static void processNode(aiNode & node, const aiScene & scene, std::shared_ptr<Model> & model);
    static Mesh processMesh(aiMesh & mesh, const aiScene & scene, const Model & model);
public:

    static std::shared_ptr<Model> get(const std::string & model);

};

#endif //ZPG_PROJEKT_MODEL_HPP
