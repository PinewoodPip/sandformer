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

        void Update() override;
    };
}
