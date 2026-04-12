#include "PhysicsSystem.hpp"

using namespace ecs::events;

namespace ecs
{
    struct AABB
    {
        float left, right, top, bottom;
    };
    static AABB GetAABB(Vector2 pos, BoundingBoxComponent* box)
    {
        return {
            pos.x + box->offset.x,
            pos.x + box->offset.x + box->size.x,
            pos.y + box->offset.y,
            pos.y + box->offset.y + box->size.y,
        };
    }

    void PhysicsSystem::Update()
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

            // Check for collisions and push away if they also have the Solid component
            for (auto [otherEntity, otherBox, otherTransform] : world->GetEntities<BoundingBoxComponent, TransformComponent>())
            {
                if (otherBox == boundingBox)
                {
                    continue;
                }

                // Get world-space AABBs
                AABB a = GetAABB(transform->position, boundingBox);
                AABB b = GetAABB(otherTransform->position, otherBox);

                // Skip if there's no overlap
                if (((a.left >= b.right) || (a.right <= b.left) || (a.top >= b.bottom) || (a.bottom <= b.top)))
                {
                    continue;
                }

                // Throw collision event
                world->EmitEvent(Collision{ entity, otherEntity });

                // Push away if the other entity is solid
                if (otherEntity->GetComponent<SolidComponent>())
                {
                    // Use the pre-move position to determine which side to push to
                    // This also prevents overshooting collisions.
                    AABB prev = GetAABB(prevPosition, boundingBox);

                    if (prev.bottom <= b.top)
                    {
                        transform->position.y = b.top - boundingBox->offset.y - boundingBox->size.y;
                        transform->velocity.y = 0.0f;
                        physics->isGrounded = true;
                    }
                    else if (prev.top >= b.bottom)
                    {
                        transform->position.y = b.bottom - boundingBox->offset.y;
                        transform->velocity.y = 0.0f;
                    }
                    else if (prev.right <= b.left)
                    {
                        transform->position.x = b.left - boundingBox->offset.x - boundingBox->size.x;
                    }
                    else if (prev.left >= b.right)
                    {
                        transform->position.x = b.right - boundingBox->offset.x;
                    }
                }
            }
        }
    }
}
