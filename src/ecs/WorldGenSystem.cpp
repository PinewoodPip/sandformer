#include "WorldGenSystem.hpp"

using namespace ecs;

void WorldGenSystem::OnStart()
{
    // Create ground
    for (int i = 0; i < 10; i++)
    {
        world->EmitEvent(RequestBlockCreateEvent{ GridPos{ i, 4 } });
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
