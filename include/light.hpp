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
    virtual void colorChanged(glm::vec3 color, LightType lightType) = 0;
    virtual void positionChanged(glm::vec3 position, LightType lightType) = 0;
};

class ColoredLight {

protected:

    glm::vec3 color { 1.f };
    std::vector<std::reference_wrapper<LightObserver>> observers;

    void onColorChanged(LightType lightType);

public:
    ColoredLight();
    ColoredLight(glm::vec3 color);

    virtual void addColor(glm::vec3 delta);
    virtual void setColor(glm::vec3 color);

    virtual void addObserver(LightObserver & observer);
    virtual void apply();

};

class PositionedLight : public ColoredLight {
    glm::vec3 color { 1.f };
    glm::vec3 position { 0.f };

    void onPositionChanged(LightType lightType);

public:
    PositionedLight();
    PositionedLight(glm::vec3 color, glm::vec3 position);

    void move(glm::vec3 delta);
    void setPosition(glm::vec3 newPos);

    void setColor(glm::vec3 color) override;

    void apply() override;
};

typedef ColoredLight AmbientLight;


#endif //ZPG_PROJEKT_LIGHT_HPP
