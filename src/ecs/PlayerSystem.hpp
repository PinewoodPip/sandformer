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

		void Update() override;
	};
}
