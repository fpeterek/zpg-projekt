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

protected:

    Texture(GLuint id, std::string texPath);
    const GLuint textureId;

public:

    const std::string texturePath;

    explicit Texture(const std::string& texturePath);

    virtual void bind(Shader & shader) const;

};

class CubeMap : public Texture {

public:
    explicit CubeMap(std::string) = delete;
    explicit CubeMap(const std::vector<std::string> & textures);

    void bind(Shader & shader) const override;

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
    static std::shared_ptr<Texture> cubeMap(const std::string & cubemap, const std::vector<std::string> & textures);

};

#endif //ZPG_PROJEKT_TEXTURE_HPP
