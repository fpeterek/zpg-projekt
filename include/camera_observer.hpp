//
// Created by fpeterek on 20.10.20.
//

#ifndef ZPG_PROJEKT_CAMERA_OBSERVER_HPP
#define ZPG_PROJEKT_CAMERA_OBSERVER_HPP

#include <glm/mat4x4.hpp>

struct CameraObserver {

    virtual void updateView(const glm::mat4 & view) = 0;
    virtual void updateProjection(const glm::mat4 & projection) = 0;
    virtual void updatePosition(const glm::vec3 & position) = 0;

};

#endif //ZPG_PROJEKT_CAMERA_OBSERVER_HPP

