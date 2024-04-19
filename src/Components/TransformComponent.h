#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>
#include "../ECS/ECS.h"
using vec2 = glm::vec2;

struct TransformComponent {
    vec2 position;
    vec2 scale;
    double rotation;

    TransformComponent(
        vec2 position = vec2(0,0),
        vec2 scale = vec2(0,0),
        double rotation = 0.0
    ) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
    }
};

#endif