#include "WorldGenSystem.hpp"

using namespace ecs;

void WorldGenSystem::OnStart()
{
    // Create ground
    for (int i = 0; i < 10; i++)
    {
        CreateBlock(Vector2{ i * (float)BLOCK_SIZE, 300.0f });
    }

    // Create coins
    for (int i = 0; i < 6; i++)
    {
        CreateCoin(Vector2{ 50 + i * (float)BLOCK_SIZE, 100.0f } );
    }
}

void WorldGenSystem::CreateBlock(Vector2 position)
{
    Entity* ground = world->CreateEntity();
    world->AddComponent(ground, ecs::TransformComponent{position});
    world->AddComponent(ground, ecs::BoundingBoxComponent{
        Vector2{BLOCK_SIZE, BLOCK_SIZE},
        Vector2{0, 0},
        });
    world->AddComponent(ground, ecs::TextureComponent{ "resources/grass.png", Vector2{ BLOCK_SIZE, BLOCK_SIZE } });
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
