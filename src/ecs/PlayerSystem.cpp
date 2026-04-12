#include "PlayerSystem.hpp"

namespace ecs
{
    void PlayerSystem::OnStart()
    {
        Entity* player = world->CreateEntity();
        world->AddComponent(player, TransformComponent{});
        world->AddComponent(player, BoundingBoxComponent{
            Vector2{PLAYER_SIZE, PLAYER_SIZE},
            Vector2{0.0f, 0.0f},
            });
        world->AddComponent(player, PlayerComponent{});
        world->AddComponent(player, PhysicsComponent{});
        world->AddComponent(player, TextureComponent{ "resources/placeholder.png", {PLAYER_SIZE, PLAYER_SIZE} });
        world->AddComponent(player, InventoryComponent{});
    }

    void PlayerSystem::Update()
    {
        for (auto [_, player, transform, physics] : world->GetEntities<PlayerComponent, TransformComponent, PhysicsComponent>())
        {
            // Handle horizontal movement
            float targetXSpeed = 0.0f; // TODO also support arrow keys
            if (IsKeyDown(KEY_A))
            {
                targetXSpeed = -MAX_SPEED;
            }
            else if (IsKeyDown(KEY_D))
            {
                targetXSpeed = MAX_SPEED;
            }

            if (targetXSpeed != 0.0f)
            {
                transform->velocity.x += ACCELERATION * (targetXSpeed > 0.0f ? 1.0f : -1.0f);
                if (transform->velocity.x > MAX_SPEED)
                {
                    transform->velocity.x = MAX_SPEED;
                }
                else if (transform->velocity.x < -MAX_SPEED)
                {
                    transform->velocity.x = -MAX_SPEED;
                }
            }
            else
            {
                if (transform->velocity.x > ACCELERATION_DECAY)
                {
                    transform->velocity.x -= ACCELERATION_DECAY;
                }
                else if (transform->velocity.x < -ACCELERATION_DECAY)
                {
                    transform->velocity.x += ACCELERATION_DECAY;
                }
                else
                {
                    transform->velocity.x = 0.0f;
                }
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
