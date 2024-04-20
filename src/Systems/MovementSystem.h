#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {
    public:
        MovementSystem(){
            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>(); 
        }

        void Update(double deltatime) {
            // Loop thru all entities that the movement system is interested on
            for (Entity entity: GetSystemEntities()) {
                // Update entity's position based on the velocity component
                TransformComponent& transform = entity.GetComponent<TransformComponent>();
                const RigidBodyComponent& rigidBody = entity.GetComponent<RigidBodyComponent>();

                transform.position.x += rigidBody.velocity.x * deltatime;
                transform.position.y += rigidBody.velocity.y * deltatime;

                Logger::Log("Entity " + entity.toString() + " position is now (" + 
                    std::to_string(transform.position.x) + ", " +
                    std::to_string(transform.position.y) + ")"
                );
            }
        }
};

#endif