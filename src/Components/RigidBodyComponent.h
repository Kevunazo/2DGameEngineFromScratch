#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>
#include "../ECS/ECS.h"
using vec2 = glm::vec2;

struct RigidBodyComponent {
    vec2 velocity;

    RigidBodyComponent(vec2 velocity = vec2(0.0, 0.0)): velocity(velocity) {};
};

#endif