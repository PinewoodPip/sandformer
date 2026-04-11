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
#include "ecs/WorldGenSystem.hpp"
#include "ecs/BlockWorldSystem.hpp"

using namespace ecs;

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
    framesCounter = 0;
    finishScreen = 0;

    // Create ECS world and systems
    world = new World();

    RenderSystem* renderSystem = new RenderSystem(world);
    world->AddSystem(renderSystem);
	PlayerSystem* playerSystem = new PlayerSystem(world);
	world->AddSystem(playerSystem);
    PhysicsSystem* physicsSystem = new PhysicsSystem(world);
    world->AddSystem(physicsSystem);
    CurrencySystem* currencySystem = new CurrencySystem(world);
    world->AddSystem(currencySystem);
    WorldGenSystem* worldGenSystem = new WorldGenSystem(world);
    world->AddSystem(worldGenSystem);
    BlockWorldSystem* blockWorldSystem = new BlockWorldSystem(world);
    world->AddSystem(blockWorldSystem);

    world->Start();
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
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), SKYBLUE);
    world->Render();
}

// Gameplay Screen Unload logic
void UnloadGameplayScreen(void)
{
    delete world;
    world = nullptr;
}

// Gameplay Screen should finish?
int FinishGameplayScreen(void)
{
    return finishScreen;
}