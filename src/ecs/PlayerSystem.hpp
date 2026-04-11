#pragma once
#include "raylib.h"
#include "Entity.hpp"

namespace ecs
{
	class PlayerSystem : public System
	{
		static constexpr float MAX_SPEED = 4.0f;
		static constexpr float ACCELERATION = 0.8f; // Velocity per tick
		static constexpr float ACCELERATION_DECAY = 0.5f; // Velocity lost per tick when not moving
		static constexpr float JUMP_VELOCITY = 10.0f; // Velocity burst when starting a jump
		static constexpr float JUMP_VELOCITY_HOLD_BOOST = 0.25f; // Extra velocity per tick from holding jump button during a jump

	public:
		PlayerSystem(World* world) : System(world) {}

		void Update() override;
	};
}
