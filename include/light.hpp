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
    Ambient
};

struct LightObserver {
    virtual void colorChanged(glm::vec3 color, size_t lightIndex, LightType lightType) = 0;
    virtual void positionChanged(glm::vec3 position, size_t lightIndex, LightType lightType) = 0;
};

class ColoredLight {

protected:

    mutable glm::vec3 color { 1.f };
    std::vector<std::reference_wrapper<LightObserver>> observers;

    void onColorChanged(LightType lightType) const;

public:
    ColoredLight();
    ColoredLight(glm::vec3 color);

    virtual void addColor(glm::vec3 delta) const;
    virtual void setColor(glm::vec3 color) const;

    virtual void addObserver(LightObserver & observer);
    virtual void apply() const;

};

class PositionedLight : public ColoredLight {
    mutable glm::vec3 position { 0.f };

    void onPositionChanged(LightType lightType) const;

public:
    PositionedLight();
    PositionedLight(glm::vec3 color, glm::vec3 position);

    std::size_t lightIndex = 0;

    void move(glm::vec3 delta) const;
    void setPosition(glm::vec3 newPos) const;

    void setColor(glm::vec3 color) const override;

    void apply() const override;
};

typedef ColoredLight AmbientLight;


#endif //ZPG_PROJEKT_LIGHT_HPP
