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


std::unordered_map<std::string, Texture> TextureManager::map;

Texture & TextureManager::get(const std::string & texture) {
    auto iter = map.find(texture);

    if (iter != map.end()) {
        return iter->second;
    }

    throw TextureNotFoundException("No such texture found: '" + texture + "'");
}

Texture & TextureManager::emplace(const std::string & texture, const std::string & texturePath) {
    Texture tex { texturePath };

    auto retval = map.emplace(std::make_pair(texture, tex));
    if (not retval.second) {
        throw TextureAlreadyExistsException("Texture '" + texture + "' already exists.");
    }

    return retval.first->second;
}

Texture & TextureManager::getOrEmplace(const std::string & texture, const std::string & texturePath) {

    auto iter = map.find(texture);

    if (iter != map.end()) {
        return iter->second;
    }

    Texture tex { texturePath };
    return map.emplace(std::make_pair(texture, tex)).first->second;
}



