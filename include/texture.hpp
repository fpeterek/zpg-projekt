//
// Created by fpeterek on 29.11.20.
//

#ifndef ZPG_PROJEKT_TEXTURE_HPP
#define ZPG_PROJEKT_TEXTURE_HPP

#include <string>
#include <unordered_map>
#include <exception>
#include <stdexcept>
#include <memory>

#include <GL/glew.h>

#include "shader.hpp"


class Texture {

    const GLuint textureId;

public:

    const std::string texturePath;

    explicit Texture(std::string texturePath);

    void bind(Shader & shader) const;

};


class TextureNotFoundException : public std::runtime_error {
public:
    explicit TextureNotFoundException(const std::string & msg) noexcept;
};

class TextureAlreadyExistsException : public std::runtime_error {
public:
    explicit TextureAlreadyExistsException(const std::string & msg) noexcept;
};

class TextureManager {
    static std::unordered_map<std::string, std::shared_ptr<Texture>> map;

public:

    static std::shared_ptr<Texture> emplace(const std::string & texture, const std::string & texturePath);
    static std::shared_ptr<Texture> getOrEmplace(const std::string & texture, const std::string & texturePath);
    static std::shared_ptr<Texture> get(const std::string & texture);

};

#endif //ZPG_PROJEKT_TEXTURE_HPP
