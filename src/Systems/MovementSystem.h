#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#define SCREEN_WIDTH    600
#define SCREEN_HEIGHT   600

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
                RigidBodyComponent& rigidBody = entity.GetComponent<RigidBodyComponent>();

                transform.position.x += rigidBody.velocity.x * deltatime;
                transform.position.y += rigidBody.velocity.y * deltatime;
                transform.rotation += 20 * deltatime;

                if (static_cast<int>(transform.position.x) < 0 || static_cast<int>(transform.position.x) > SCREEN_WIDTH) {
                    // Make Velocity Negative
                    rigidBody.velocity.x *= -1;

                    Logger::Log("Entity " + entity.toString() + " velocity is now (" + 
                        std::to_string(rigidBody.velocity.x) + ", " +
                        std::to_string(rigidBody.velocity.y) + ")"
                    );
                }

                if (static_cast<int>(transform.position.y) < 0 || static_cast<int>(transform.position.y) > SCREEN_HEIGHT) {
                    // Make Velocity Negative
                    rigidBody.velocity.y *= -1;

                    Logger::Log("Entity " + entity.toString() + " velocity is now (" + 
                        std::to_string(rigidBody.velocity.x) + ", " +
                        std::to_string(rigidBody.velocity.y) + ")"
                    );
                }

                // Logger::Log("Entity " + entity.toString() + " position is now (" + 
                //     std::to_string(transform.position.x) + ", " +
                //     std::to_string(transform.position.y) + ")"
                // );

            }
        }
};

#endif