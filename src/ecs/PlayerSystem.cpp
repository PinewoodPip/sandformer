#include "PlayerSystem.hpp"

namespace ecs
{
    void PlayerSystem::Update()
    {
        for (auto [_, player, transform, physics] : world->GetEntities<PlayerComponent, TransformComponent, PhysicsComponent>())
        {
            Vector2 movement{ 0.f, 0.f }; // TODO also support arrow keys
            if (IsKeyDown(KEY_W) && physics->isGrounded)
                movement.y = -JUMP_VELOCITY;
            if (IsKeyDown(KEY_A))
                movement.x -= PLAYER_SPEED;
            else if (IsKeyDown(KEY_D))
                movement.x += PLAYER_SPEED;

            // TODO use acceleration instead
            transform->velocity.x = movement.x;
            transform->velocity.y = movement.y;
        }
    }
}
