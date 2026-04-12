#include "WorldGenSystem.hpp"

using namespace ecs::events;

namespace ecs
{
    void WorldGenSystem::OnStart()
    {
        // Create ground
        for (int i = 0; i < 13; i++)
        {
            world->EmitEvent(RequestCreateBlock{ GridPos{ i, 4 } });
            world->EmitEvent(RequestCreateBlock{ GridPos{ i, 5 }, BlockType::Dirt });
            world->EmitEvent(RequestCreateBlock{ GridPos{ i, 6 }, BlockType::Dirt });
        }

        // Create hill on the right
        for (int i = 8; i < 11; i++)
        {
            world->EmitEvent(RequestCreateBlock{ GridPos{ i, 3 } });
            world->EmitEvent(RequestCreateBlock{ GridPos{ i, 4 }, BlockType::Dirt });
            world->EmitEvent(RequestCreateBlock{ GridPos{ i, 5 }, BlockType::Dirt });
            world->EmitEvent(RequestCreateBlock{ GridPos{ i, 6 }, BlockType::Dirt });
        }

        // Create coins
        for (int i = 0; i < 6; i++)
        {
            CreateCoin(Vector2{ (1 + i) * BLOCK_SIZE, 1 * BLOCK_SIZE });
        }
    }

    void WorldGenSystem::CreateCoin(Vector2 position)
    {
        Entity* coin = world->CreateEntity();
        world->AddComponent(coin, TransformComponent{ position });
        world->AddComponent(coin, BoundingBoxComponent{
            Vector2{ COIN_SIZE, COIN_SIZE },
            Vector2{ 0.0f, 0.0f },
            });
        world->AddComponent(coin, CurrencyComponent{ CurrencyType::Coin, 1 });
        world->AddComponent(coin, TextureComponent{ "resources/coin.png", Vector2{ COIN_SIZE, COIN_SIZE } });
    }
}
