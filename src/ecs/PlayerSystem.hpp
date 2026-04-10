#pragma once
#include "raylib.h"
#include "Entity.hpp"

namespace ecs
{
	class PlayerSystem : public System
	{
		static constexpr float PLAYER_SPEED = 3.0f;
		static constexpr float JUMP_VELOCITY = 200.0f;

	public:
		PlayerSystem(World* world) : System(world) {}

		void Update() override
		{
			for (Entity* entity : world->GetEntities()) // TODO add a filtered GetEntities() call
			{
				PlayerComponent* player = entity->GetComponent<PlayerComponent>();
				if (player)
				{
					TransformComponent* transform = entity->GetComponent<TransformComponent>();
					PhysicsComponent* physics = entity->GetComponent<PhysicsComponent>();

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
	};
}
