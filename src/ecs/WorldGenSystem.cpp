#include "WorldGenSystem.hpp"

using namespace ecs;

void WorldGenSystem::OnStart()
{
    // Create ground
    for (int i = 0; i < 13; i++)
    {
        world->EmitEvent(RequestBlockCreateEvent{ GridPos{ i, 4 } });
        world->EmitEvent(RequestBlockCreateEvent{ GridPos{ i, 5 }, BlockType::Dirt });
        world->EmitEvent(RequestBlockCreateEvent{ GridPos{ i, 6 }, BlockType::Dirt });
    }

    // Create hill on the right
    for (int i = 8; i < 11; i++)
    {
        world->EmitEvent(RequestBlockCreateEvent{ GridPos{ i, 3 } });
        world->EmitEvent(RequestBlockCreateEvent{ GridPos{ i, 4 }, BlockType::Dirt });
        world->EmitEvent(RequestBlockCreateEvent{ GridPos{ i, 5 }, BlockType::Dirt });
        world->EmitEvent(RequestBlockCreateEvent{ GridPos{ i, 6 }, BlockType::Dirt });
    }

    // Create coins
    for (int i = 0; i < 6; i++)
    {
        CreateCoin(Vector2{ (1 + i) * (float)BLOCK_SIZE, 1 * (float)BLOCK_SIZE });
    }
}

void WorldGenSystem::CreateCoin(Vector2 position)
{
    Entity* coin = world->CreateEntity();
    world->AddComponent(coin, ecs::TransformComponent{ position });
    world->AddComponent(coin, ecs::BoundingBoxComponent{
        Vector2{ COIN_SIZE, COIN_SIZE },
        Vector2{ 0, 0 },
        });
    world->AddComponent(coin, ecs::CurrencyComponent{ CurrencyType::Coin, 1 });
    world->AddComponent(coin, ecs::TextureComponent{ "resources/coin.png", Vector2{ COIN_SIZE, COIN_SIZE } });
}
