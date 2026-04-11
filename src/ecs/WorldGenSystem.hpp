#include "World.hpp"

namespace ecs
{
    class WorldGenSystem : public System
    {
    public:
        static constexpr int BLOCK_SIZE = 64;
        static constexpr int COIN_SIZE = 32;

        WorldGenSystem(World* world) : System(world) {}

        void OnStart() override;

    private:
        void CreateCoin(Vector2 position);
    };
}