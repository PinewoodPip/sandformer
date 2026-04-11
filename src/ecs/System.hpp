#pragma once

#include "Events.hpp"

namespace ecs {

    class World;

    // Base class for ECS systems.
    class System
    {
    public:
        virtual void OnStart() {}; // Called when the ECS world starts, use for setup routines
        virtual void ProcessEvent(const events::AnyEvent& event) {};
        virtual void Update() {}; // Called every tick
        virtual void Render() {}; // Intended to draw graphics using raylib

        System(World* world) : world(world) {}

        virtual ~System() = default;

    protected:
        World* world;
    };
}
