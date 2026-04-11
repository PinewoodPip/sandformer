#include "PlayerSystem.hpp"

namespace ecs
{
    void PlayerSystem::Update()
    {
        for (auto [_, player, transform, physics] : world->GetEntities<PlayerComponent, TransformComponent, PhysicsComponent>())
        {
            // Handle horizontal movement
            float targetXSpeed = 0.0f; // TODO also support arrow keys
            if (IsKeyDown(KEY_A))
                targetXSpeed = -MAX_SPEED;
            else if (IsKeyDown(KEY_D))
                targetXSpeed = MAX_SPEED;

            if (targetXSpeed != 0.0f)
            {
                transform->velocity.x += ACCELERATION * (targetXSpeed > 0.0f ? 1.0f : -1.0f);
                if (transform->velocity.x > MAX_SPEED)
                    transform->velocity.x = MAX_SPEED;
                else if (transform->velocity.x < -MAX_SPEED)
                    transform->velocity.x = -MAX_SPEED;
            }
            else
            {
                if (transform->velocity.x > ACCELERATION_DECAY)
                    transform->velocity.x -= ACCELERATION_DECAY;
                else if (transform->velocity.x < -ACCELERATION_DECAY)
                    transform->velocity.x += ACCELERATION_DECAY;
                else
                    transform->velocity.x = 0.0f;
            }

            // Handle jump physics
            // Initial press gives burst of velocity, holding it lets the player jump higher
            // Roughly the NES Mario jump
            if (IsKeyPressed(KEY_W) && physics->isGrounded)
            {
                transform->velocity.y = -JUMP_VELOCITY;
            }
            else if (IsKeyDown(KEY_W) && (transform->velocity.y < 0.0f))
            {
                transform->velocity.y -= JUMP_VELOCITY_HOLD_BOOST;
            }
        }
    }
}
