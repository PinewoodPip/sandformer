/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.hpp"
#include "ecs/Entity.hpp"
#include "ecs/PlayerSystem.hpp"
#include "ecs/PhysicsSystem.hpp"
#include "ecs/CurrencySystem.hpp"
#include "ecs/RenderSystem.hpp"

using namespace ecs;

const int BLOCK_SIZE = 64;
const int COIN_SIZE = 32;

//----------------------------------------------------------------------------------
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
static int framesCounter = 0;
static int finishScreen = 0;


static ecs::World* world = nullptr;

//----------------------------------------------------------------------------------
// Gameplay Screen Functions Definition
//----------------------------------------------------------------------------------

// Gameplay Screen Initialization logic
void InitGameplayScreen(void)
{
    // TODO: Initialize GAMEPLAY screen variables here!
    framesCounter = 0;
    finishScreen = 0;

    world = new World();

    // Create player
    Entity* player = world->CreateEntity();
    world->AddComponent(player, ecs::TransformComponent{});
    world->AddComponent(player, ecs::BoundingBoxComponent{
        Vector2{1, 1},
        Vector2{0, 0},
        });
	world->AddComponent(player, ecs::PlayerComponent{});
    world->AddComponent(player, ecs::PhysicsComponent{});
    //LoadTexture("resources/mecha.png"); // TODO!
    world->AddComponent(player, ecs::TextureComponent{ "resources/placeholder.png" });
    world->AddComponent(player, ecs::InventoryComponent{});

    // Create systems
    RenderSystem* renderSystem = new RenderSystem(world);
    world->AddSystem(renderSystem);
	PlayerSystem* playerSystem = new PlayerSystem(world);
	world->AddSystem(playerSystem);
    PhysicsSystem* physicsSystem = new PhysicsSystem(world);
    world->AddSystem(physicsSystem);
    CurrencySystem* currencySystem = new CurrencySystem(world);
    world->AddSystem(currencySystem);

    // Create ground
    for (int i = 0; i < 10; i++)
    {
        Entity* ground = world->CreateEntity();
        world->AddComponent(ground, ecs::TransformComponent{ Vector2{ i * (float)BLOCK_SIZE, 300.0f } });
        world->AddComponent(ground, ecs::BoundingBoxComponent{
            Vector2{BLOCK_SIZE, BLOCK_SIZE},
            Vector2{0, 0},
            });
		world->AddComponent(ground, ecs::TextureComponent{ "resources/grass.png", Vector2{ BLOCK_SIZE, BLOCK_SIZE } });
    }

    // Create coins
    for (int i = 0; i < 6; i++)
    {
        Entity* coin = world->CreateEntity();
        world->AddComponent(coin, ecs::TransformComponent{ Vector2{ 50 + i * (float)BLOCK_SIZE, 100.0f } });
        world->AddComponent(coin, ecs::BoundingBoxComponent{
            Vector2{COIN_SIZE, COIN_SIZE},
            Vector2{0, 0},
            });
		world->AddComponent(coin, ecs::CurrencyComponent{ CurrencyType::Coin, 1 });
		world->AddComponent(coin, ecs::TextureComponent{ "resources/coin.png", Vector2{ COIN_SIZE, COIN_SIZE } });
    }
}

// Gameplay Screen Update logic
void UpdateGameplayScreen(void)
{
    // TODO: Update GAMEPLAY screen variables here!

    // Press enter or tap to change to ENDING screen
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1;
        //PlaySound(fxCoin);
    }

    world->Update();
}

// Gameplay Screen Draw logic
void DrawGameplayScreen(void)
{
    // TODO: Draw GAMEPLAY screen here!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), PURPLE);
    Vector2 pos = { 20, 10 };
    //DrawTextEx(font, "GAMEPLAY SCREEN", pos, font.baseSize*3.0f, 4, MAROON);
    DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 130, 220, 20, MAROON);

    world->Render();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    // TODO: Unload GAMEPLAY screen variables here!
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}