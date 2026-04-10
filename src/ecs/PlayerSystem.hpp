#pragma once
#include "raylib.h"
#include "Entity.hpp"

namespace ecs
{
	class PlayerSystem : public System
	{
		static constexpr float PLAYER_SPEED = 3.0f;

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

					Vector2 movement{ 0.f, 0.f }; // TODO also support arrow keys
					if (IsKeyDown(KEY_W))
						movement.y -= PLAYER_SPEED;
					else if (IsKeyDown(KEY_S))
						movement.y += PLAYER_SPEED;
					if (IsKeyDown(KEY_A))
						movement.x -= PLAYER_SPEED;
					else if (IsKeyDown(KEY_D))
						movement.x += PLAYER_SPEED;

					transform->position.x += movement.x;
					transform->position.y += movement.y;
				}
			}
		}
	};
}
