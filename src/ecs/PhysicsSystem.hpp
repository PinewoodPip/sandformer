#pragma once
#include "raylib.h"
#include "Entity.hpp"
#include <algorithm>

namespace ecs
{
    class PhysicsSystem : public System
    {
        static constexpr float GRAVITY = 0.5f;

    public:
        PhysicsSystem(World* world) : System(world) {}

        void Update() override
        {
            std::vector<Entity*> collidableEntities; // TODO optimize by caching entities with gravity and bounding boxes

            for (Entity* entity : world->GetEntities())
            {
                if (entity->GetComponent<BoundingBoxComponent>())
                    collidableEntities.push_back(entity);
            }

            for (Entity* entity : world->GetEntities())
            {
                GravityComponent* gravity = entity->GetComponent<GravityComponent>();
                if (!gravity)
                    continue;

                BoundingBoxComponent* boundingBox = entity->GetComponent<BoundingBoxComponent>();
                TransformComponent* transform = entity->GetComponent<TransformComponent>();
                if (!boundingBox || !transform)
                    continue;

                // Apply gravity
                transform->position.y += GRAVITY;

                // Check for collisions and push away from collidables
                for (Entity* otherEntity : collidableEntities)
                {
                    if (otherEntity == entity)
                        continue;

                    BoundingBoxComponent* otherBox = otherEntity->GetComponent<BoundingBoxComponent>();
                    TransformComponent* otherTransform = otherEntity->GetComponent<TransformComponent>();
                    if (!otherBox || !otherTransform)
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

                    // Continue if there's no overlap
                    if (aLeft >= bRight || aRight <= bLeft || aTop >= bBottom || aBottom <= bTop)
                        continue;

                    // Get penetration depth to determine axis with least collision
                    float overlapX = std::min(aRight - bLeft, bRight - aLeft);
                    float overlapY = std::min(aBottom - bTop, bBottom - aTop);

                    // Push away from the axis
                    if (overlapX < overlapY)
                    {
                        if (aLeft < bLeft)
                            transform->position.x -= overlapX;
                        else
                            transform->position.x += overlapX;
                    }
                    else
                    {
                        if (aTop < bTop)
                            transform->position.y -= overlapY;
                        else
                            transform->position.y += overlapY;
                    }

                    // Throw event
                    world->EmitEvent(CollisionEvent{ entity, otherEntity });
                }
            }
        }
    };
}
