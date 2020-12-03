//
// Created by fpeterek on 02.11.20.
//

#include "model.hpp"

#include <cmath>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


std::unordered_map<std::string, std::shared_ptr<Model>> ModelLoader::models;


void Mesh::bindAndDraw(const unsigned int id, Shader & shader) const {

    if (texture) {
        texture->bind(shader);
    }

    glStencilFunc(GL_ALWAYS, id, 0xFF);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::shared_ptr<Texture> tex, const Material & material) :
    vertices(std::move(vertices)), indices(std::move(indices)), texture(std::move(tex)), material(material) {

    init();

}

void Mesh::init() {

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(decltype(vertices)::value_type), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(decltype(indices)::value_type), indices.data(), GL_STATIC_DRAW);

    /* Vertex positions */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(decltype(vertices)::value_type), nullptr);

    /* Vertex normals */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(decltype(vertices)::value_type), (void*)sizeof(Vertex::position));

    /* Texture coordinates */
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(decltype(vertices)::value_type),
                          (void*)( sizeof(Vertex::position) + sizeof(Vertex::normal)) );

    glBindVertexArray(0);

}

Mesh::Mesh(Mesh && mesh) noexcept :
    vertices(std::move(mesh.vertices)), indices(std::move(mesh.indices)), texture(std::move(mesh.texture)),
    vbo(mesh.vbo), vao(mesh.vao), ebo(mesh.ebo), material(mesh.material) { }


std::shared_ptr<Model> ModelLoader::get(const std::string & key) {

    auto iter = models.find(key);

    if (iter != models.end()) {
        return iter->second;
    }
    return loadModel(key);
}

static glm::vec3 extractVector(aiVector3D & vec) {
    return glm::vec3 { vec.x, vec.y, vec.z };
}

static void processVertices(aiMesh & mesh, std::vector<Vertex> & vertices) {
    for (uint32_t i = 0; i < mesh.mNumVertices; ++i) {
        vertices.emplace_back();
        vertices.back().position = extractVector(mesh.mVertices[i]);
        vertices.back().normal = extractVector(mesh.mNormals[i]);
        if (mesh.mTextureCoords[0]) {
            auto vec = extractVector(mesh.mTextureCoords[0][i]);
            vertices.back().texCoords = glm::vec2 { vec.x, vec.y };
        } else {
            vertices.back().texCoords = glm::vec2 { 0.f, 0.f };
        }
    }
}

static void processIndices(aiMesh & mesh, std::vector<uint32_t> & indices) {
    for (uint32_t i = 0; i < mesh.mNumFaces; ++i) {
        aiFace face = mesh.mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; ++j) {
            indices.emplace_back(face.mIndices[j]);
        }
    }
}

static glm::vec3 toVector(const aiColor3D & color) {
    return glm::vec3 { color.r, color.g, color.b };
}

static Material loadMaterial(aiMaterial & mat, Material & material) {
    aiColor3D color { 0.f, 0.f, 0.f };

    mat.Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = toVector(color);

    mat.Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = toVector(color);

    mat.Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = toVector(color);

    mat.Get(AI_MATKEY_SHININESS, material.shininess);

    aiString path;

    if (mat.GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
        material.diffuseMap = path.C_Str();
    }
    if (mat.GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
        material.specularMap = path.C_Str();
    }
    if (mat.GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS) {
        material.heightMap = path.C_Str();
    }

    return material;
}

static void processMaterials(const aiScene & scene, std::vector<Material> & materials) {

    for (uint32_t i = 0; i < scene.mNumMaterials; ++i) {
        materials.emplace_back();
        aiMaterial & mat = *scene.mMaterials[i];
        loadMaterial(mat, materials.back());
    }

}

static std::string dewindows(std::string str) {

    int bsCounter = 0;

    for (size_t i = 0; i < str.size(); ++i) {

        if (str[i] == '\\' and not bsCounter) {
            str[i] = '/';
            ++bsCounter;
        } else if (str[i] == '\\' and bsCounter) {
            str.erase(str.begin() + i);
            bsCounter = 0;
            --i;
        }
    }

    return str;
}

std::string lufthansa(std::string str) {

    std::string factory = "Factory Livery";
    if (str.find(factory) == 0) {
        str = "Lufthansa" + str.substr(factory.size());
    }
    return str;

}

Mesh ModelLoader::processMesh(aiMesh & mesh, const aiScene & scene, const Model & model) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    processVertices(mesh, vertices);
    processIndices(mesh, indices);
    const Material & mat = model.materials[mesh.mMaterialIndex];

    std::shared_ptr<Texture> texture = nullptr;
    if (not mat.diffuseMap.empty()) {
        std::string tex = dewindows(model.directory + "/" + lufthansa(mat.diffuseMap));
        std::cout << tex << std::endl;
        texture = TextureManager::getOrEmplace(tex, tex);
    }

    return Mesh(std::move(vertices), std::move(indices), texture, mat);
}

void ModelLoader::processNode(aiNode & node, const aiScene & scene, std::shared_ptr<Model> & model) {
    for (uint32_t i = 0; i < node.mNumMeshes; ++i) {
        aiMesh * mesh = scene.mMeshes[node.mMeshes[i]];
        model->addMesh(processMesh(*mesh, scene, *model));
    }
    for (uint32_t i = 0; i < node.mNumChildren; ++i) {
        processNode(*(node.mChildren[i]), scene, model);
    }
}

std::shared_ptr<Model> ModelLoader::loadModel(const std::string & key) {
    auto path = formatName(key);

    Assimp::Importer importer;
    const aiScene * aiscene = importer.ReadFile(path, importOpts);

    if (!aiscene) {
        throw std::runtime_error("Error loading model '" + key + "'");
    }

    const aiScene & scene = *aiscene;

    if (scene.mFlags bitand AI_SCENE_FLAGS_INCOMPLETE or not scene.mRootNode) {
        throw std::runtime_error("Error loading model '" + key + "'");
    }

    auto model = std::make_shared<Model>();
    model->directory = path.substr(0, path.find_last_of('/'));
    processMaterials(scene, model->materials);
    processNode(*scene.mRootNode, scene, model);

    return model;
}

std::string ModelLoader::formatName(const std::string & model) {
    return "resources/models/" + model + ".obj";
}

void Model::draw(const uint32_t id, Shader & shader) const {
    for (const Mesh & mesh : meshes) {
        mesh.bindAndDraw(id, shader);
    }
}

void Model::addMesh(Mesh && mesh) {
    meshes.emplace_back(std::move(mesh));
}
