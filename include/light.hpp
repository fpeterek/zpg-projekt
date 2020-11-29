//
// Created by fpeterek on 25.10.20.
//

#ifndef ZPG_PROJEKT_LIGHT_HPP
#define ZPG_PROJEKT_LIGHT_HPP

#include <vector>
#include <functional>

#include <glm/vec3.hpp>

enum class LightType {
    Default,
    Directional,
    Ambient
};

namespace gl {
    enum class Light {
        Ambient = 0,
        Point = 1,
        Directional = 2,
        Spotlight = 3
    };
}

struct LightObserver {
    virtual void colorChanged(glm::vec3 color, size_t lightIndex, LightType lightType) = 0;
    virtual void positionChanged(glm::vec3 position, size_t lightIndex, LightType lightType) = 0;
    virtual void typeChanged(gl::Light type, size_t lightIndex) = 0;
};

class ColoredLight {

protected:

    mutable glm::vec3 color { 1.f };
    std::vector<std::reference_wrapper<LightObserver>> observers;

    void onColorChanged(LightType lightType) const;

public:
    ColoredLight();
    ColoredLight(glm::vec3 color);

    std::size_t lightIndex = 0;

    virtual gl::Light type() const;

    virtual void addColor(glm::vec3 delta) const;
    virtual void setColor(glm::vec3 color) const;

    virtual void addObserver(LightObserver & observer);
    virtual void apply() const;

};

class PositionedLight : public ColoredLight {
    mutable glm::vec3 position { 0.f };

    void onPositionChanged(LightType lightType) const;

public:

    gl::Light type() const override;

    PositionedLight();
    PositionedLight(glm::vec3 color, glm::vec3 position);

    void move(glm::vec3 delta) const;
    void setPosition(glm::vec3 newPos) const;

    void setColor(glm::vec3 color) const override;

    void apply() const override;
};

typedef ColoredLight AmbientLight;


class DirectionalLight : public ColoredLight {
    mutable glm::vec3 direction { 0.f };

    void onDirectionChange(LightType lightType) const;

public:

    gl::Light type() const override;

    DirectionalLight();
    DirectionalLight(glm::vec3 color, glm::vec3 direction);

    void move(glm::vec3 delta) const;
    void setDirection(glm::vec3 newDirection) const;

    void setColor(glm::vec3 color) const override;

    void apply() const override;
};

#endif //ZPG_PROJEKT_LIGHT_HPP
