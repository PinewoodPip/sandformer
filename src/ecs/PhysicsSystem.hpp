#pragma once
#include "raylib.h"
#include "Entity.hpp"

namespace ecs
{
    class PhysicsSystem : public System
    {
        static constexpr float GRAVITY = 3.0f; // Downwards velocity applied per-tick
        static constexpr float MAX_FALL_SPEED = 70.0f;

    public:
        PhysicsSystem(World* world) : System(world) {}

        void Update() override
        {
            for (auto [entity, physics, boundingBox, transform] : world->GetEntities<PhysicsComponent, BoundingBoxComponent, TransformComponent>())
            {
                // Apply gravity
                transform->velocity.y += GRAVITY;
                if (transform->velocity.y > MAX_FALL_SPEED)
                {
                    transform->velocity.y = MAX_FALL_SPEED;
                }

                // Record position before moving so we know the side we collide from
                Vector2 prevPosition = transform->position;

                // Apply velocity
                transform->position.x += transform->velocity.x;
                transform->position.y += transform->velocity.y;

                physics->isGrounded = false; // Will be re-set after checking collision

                // Check for collisions and push away
                for (auto [otherEntity, otherBox, otherTransform] : world->GetEntities<BoundingBoxComponent, TransformComponent>())
                {
                    if (otherBox == boundingBox)
                        continue;

                    // Get world-space AABBs
                    float aLeft = transform->position.x + boundingBox->offset.x;
                    float aRight = aLeft + boundingBox->size.x;
                    float aTop = transform->position.y + boundingBox->offset.y;
                    float aBottom = aTop + boundingBox->size.y;

                    float bLeft = otherTransform->position.x + otherBox->offset.x;
                    float bRight = bLeft + otherBox->size.x;
                    float bTop = otherTransform->position.y + otherBox->offset.y;
                    float bBottom = bTop + otherBox->size.y;

                    // Skip if there's no overlap
                    if (aLeft >= bRight || aRight <= bLeft || aTop >= bBottom || aBottom <= bTop)
                        continue;

                    // Use the pre-move position to determine which side to push to
                    // This also prevents overshooting collisions.
                    float prevBottom = prevPosition.y + boundingBox->offset.y + boundingBox->size.y;
                    float prevTop = prevPosition.y + boundingBox->offset.y;
                    float prevRight = prevPosition.x + boundingBox->offset.x + boundingBox->size.x;
                    float prevLeft = prevPosition.x + boundingBox->offset.x;

                    if (prevBottom <= bTop)
                    {
                        transform->position.y = bTop - boundingBox->offset.y - boundingBox->size.y;
                        transform->velocity.y = 0.0f;
                        physics->isGrounded = true;
                    }
                    else if (prevTop >= bBottom)
                    {
                        transform->position.y = bBottom - boundingBox->offset.y;
                        transform->velocity.y = 0.0f;
                    }
                    else if (prevRight <= bLeft)
                    {
                        transform->position.x = bLeft - boundingBox->offset.x - boundingBox->size.x;
                    }
                    else if (prevLeft >= bRight)
                    {
                        transform->position.x = bRight - boundingBox->offset.x;
                    }

                    world->EmitEvent(CollisionEvent{ entity, otherEntity });
                }
            }
        }
    };
}
