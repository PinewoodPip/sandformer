#include "BlockWorldSystem.hpp"
#include "World.hpp"

using namespace ecs;
using namespace ecs::events;

void BlockWorldSystem::Update()
{
    // Break blocks with left-click
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        BreakBlockAtPos(GetMousePosition());
    }

    // Place blocks with right-click
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        Vector2 mousePos = GetMousePosition();
        Vector2 mouseGridPos = {
            (float)((int)(mousePos.x / BLOCK_SIZE) * BLOCK_SIZE),
            (float)((int)(mousePos.y / BLOCK_SIZE) * BLOCK_SIZE),
        };
        TryPlaceBlock(BlockType::Grass, mouseGridPos);
    }
}

void BlockWorldSystem::BreakBlockAtPos(Vector2 pos)
{
    for (auto [entity, transform, bbox, block] : world->GetEntities<TransformComponent, BoundingBoxComponent, BlockComponent>())
    {
        Rectangle blockRect = {
            transform->position.x + bbox->offset.x,
            transform->position.y + bbox->offset.y,
            bbox->size.x,
            bbox->size.y,
        };
        if (CheckCollisionPointRec(pos, blockRect))
        {
            world->EmitEvent(RequestDestroyEntity{ entity });
            break; // There can only be 1 block under the cursor
        }
    }
}

bool BlockWorldSystem::TryPlaceBlock(BlockType blockType, Vector2 pos)
{
    // Don't place if a block or other solid already occupies this cell
    // TODO refactor this system to better keep track of occupied "cells"
    for (auto [entity, transform, bbox, _] : world->GetEntities<TransformComponent, BoundingBoxComponent, SolidComponent>())
    {
        if ((transform->position.x == pos.x) && (transform->position.y == pos.y))
        {
            return false;
        }
    }
    PlaceBlock(blockType, pos);
    return true;
}

void BlockWorldSystem::ProcessEvent(const AnyEvent& event)
{
    // Handle requests to place blocks
    if (const auto* e = std::get_if<RequestCreateBlock>(&event))
    {
        TryPlaceBlock(e->blockType, Vector2{
            e->pos.x * (float)BLOCK_SIZE,
            e->pos.y * (float)BLOCK_SIZE,
        });
    }
}

void BlockWorldSystem::PlaceBlock(BlockType blockType, Vector2 position)
{
    BlockDescriptor desc = GetBlockDescriptor(blockType);
    Entity* block = world->CreateEntity();
    world->AddComponent(block, TransformComponent{ position });
    world->AddComponent(block, BoundingBoxComponent{
        Vector2{ BLOCK_SIZE, BLOCK_SIZE },
        Vector2{ 0.0f, 0.0f },
    });
    world->AddComponent(block, SolidComponent{});
    world->AddComponent(block, BlockComponent{ blockType });
    world->AddComponent(block, TextureComponent{ desc.texturePath, Vector2{ BLOCK_SIZE, BLOCK_SIZE } });
}
