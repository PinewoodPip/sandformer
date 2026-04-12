#pragma once
#include "World.hpp"

namespace ecs
{
    class WorldGenSystem : public System
    {
    public:
        static constexpr float BLOCK_SIZE = 64.0f;
        static constexpr float COIN_SIZE = 32.0f;

        WorldGenSystem(World* world) : System(world) {}

        void OnStart() override;

    private:
        void CreateCoin(Vector2 position);
    };
}
