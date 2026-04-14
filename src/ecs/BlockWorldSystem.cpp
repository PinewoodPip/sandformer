#include "BlockWorldSystem.hpp"
#include "World.hpp"

using namespace ecs::events;

namespace ecs
{
    void BlockWorldSystem::OnStart()
    {
        // Handle requests to place blocks
        world->RegisterListener<RequestCreateBlock>([this](const RequestCreateBlock& request)
        {
            TryPlaceBlock(request.blockType, Vector2{
                request.pos.x * BLOCK_SIZE,
                request.pos.y * BLOCK_SIZE,
            });
        });
    }

    void BlockWorldSystem::Update()
    {
        // Break blocks with left-click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            BreakBlockAtPos(GetMousePosition());
        }

        // Place blocks with right-click using the selected hotbar slot
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            auto [_, player, hotbar] = world->GetEntity<PlayerComponent, HotbarComponent>().value();
            BlockType selected = hotbar->slots[hotbar->selectedIndex];
            if (selected != BlockType::None)
            {
                Vector2 mousePos = GetMousePosition();
                Vector2 mouseGridPos = {
                    (float)((int)(mousePos.x / BLOCK_SIZE) * BLOCK_SIZE),
                    (float)((int)(mousePos.y / BLOCK_SIZE) * BLOCK_SIZE),
                };
                TryPlaceBlock(selected, mouseGridPos);
            }
        }
    }

    void BlockWorldSystem::Render()
    {
        // Draw outline around the selected grid position
        Vector2 mousePos = GetMousePosition();
        Vector2 gridPos = {
            (float)((int)(mousePos.x / BLOCK_SIZE) * BLOCK_SIZE),
            (float)((int)(mousePos.y / BLOCK_SIZE) * BLOCK_SIZE),
        };
        DrawRectangleLinesEx(
            Rectangle{ gridPos.x, gridPos.y, BLOCK_SIZE, BLOCK_SIZE },
            2.0f,
            GREEN
        );

        // Draw controls hint
        static const char* hint = "Left-click: Break block      Right-click: Place block";
        int fontSize = 20;
        int textWidth = MeasureText(hint, fontSize);
        DrawText(hint, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() - 30, fontSize, WHITE);
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
                world->PushEvent(RequestDestroyEntity{ &entity });
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
}
