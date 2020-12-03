//
// Created by fpeterek on 29.11.20.
//

#include "texture.hpp"

#include <cstdint>

#include <SOIL/SOIL.h>


static GLuint loadTexture(const std::string & texturePath) {
    return SOIL_load_OGL_texture(texturePath.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}

Texture::Texture(std::string texturePath) : textureId(loadTexture(texturePath)), texturePath(std::move(texturePath)) {

}

void Texture::bind(Shader & shader) const {
    glActiveTexture(GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, textureId);
    shader.passUniformLocation("textureUnitID", std::int32_t(0));
}


TextureNotFoundException::TextureNotFoundException(const std::string & msg) noexcept : std::runtime_error(msg) { }
TextureAlreadyExistsException::TextureAlreadyExistsException(const std::string & msg) noexcept : std::runtime_error(msg) { }


std::unordered_map<std::string, std::shared_ptr<Texture>> TextureManager::map;

std::shared_ptr<Texture> TextureManager::get(const std::string & texture) {
    auto iter = map.find(texture);

    if (iter != map.end()) {
        return iter->second;
    }

    throw TextureNotFoundException("No such texture found: '" + texture + "'");
}

std::shared_ptr<Texture> TextureManager::emplace(const std::string & texture, const std::string & texturePath) {
    auto tex = std::make_shared<Texture>(texturePath);

    auto retval = map.emplace(std::make_pair(texture, tex));
    if (not retval.second) {
        throw TextureAlreadyExistsException("Texture '" + texture + "' already exists.");
    }

    return retval.first->second;
}

std::shared_ptr<Texture> TextureManager::getOrEmplace(const std::string & texture, const std::string & texturePath) {

    auto iter = map.find(texture);

    if (iter != map.end()) {
        return iter->second;
    }

    auto tex = std::make_shared<Texture>(texturePath);
    return map.emplace(std::make_pair(texture, tex)).first->second;
}


