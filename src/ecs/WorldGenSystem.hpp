#include "World.hpp"

namespace ecs
{
    class WorldGenSystem : public System
    {
    public:
        static constexpr int BLOCK_SIZE = 64;
        static constexpr int COIN_SIZE = 32;

        // TODO remove once BlockWorldSystem is refactored to handle placing blocks
        static Vector2 SnapToGrid(Vector2 pos)
        {
            return Vector2{
                (float)((int)(pos.x / BLOCK_SIZE) * BLOCK_SIZE),
                (float)((int)(pos.y / BLOCK_SIZE) * BLOCK_SIZE),
            };
        }

        WorldGenSystem(World* world) : System(world) {}

        void OnStart() override;

    private:
        void CreateBlock(Vector2 position);
        void CreateCoin(Vector2 position);
    };
}