#include "PlayerSystem.hpp"

namespace ecs
{
    constexpr int HOTBAR_SLOT_SIZE = 32;
    constexpr int HOTBAR_SLOT_PADDING = 4;
    constexpr int HOTBAR_BOTTOM_MARGIN = 40;

    void PlayerSystem::OnStart()
    {
        Entity* player = world->CreateEntity();
        world->AddComponent(player, TransformComponent{});
        world->AddComponent(player, BoundingBoxComponent{
            Vector2{PLAYER_SIZE, PLAYER_SIZE},
            Vector2{0.0f, 0.0f},
            });
        world->AddComponent(player, PlayerComponent{});
        world->AddComponent(player, PhysicsComponent{});
        world->AddComponent(player, TextureComponent{ "resources/placeholder.png", {PLAYER_SIZE, PLAYER_SIZE} });
        world->AddComponent(player, InventoryComponent{});
        HotbarComponent hotbar{};
        hotbar.slots[0] = BlockType::Grass;
        hotbar.slots[1] = BlockType::Dirt;
        world->AddComponent(player, hotbar);
        playerEntity = player;
    }

    void PlayerSystem::OnShutdown()
    {
        world->PushEvent(events::RequestDestroyEntity{ playerEntity });
        playerEntity = nullptr;
    }

    void PlayerSystem::Update()
    {
        auto [hotbar, transform, physics] = world->GetEntityView<HotbarComponent, TransformComponent, PhysicsComponent>(playerEntity).value();

        // Handle hotbar scrolling
        float scroll = GetMouseWheelMove();
        if (scroll != 0.0f)
        {
            int newIndex = hotbar->selectedIndex - (int)scroll;
            if (newIndex < 0)
            {
                newIndex = (int)hotbar->slots.size() - 1;
            }
            else if (newIndex >= (int)hotbar->slots.size())
            {
                newIndex = 0;
            }
            hotbar->selectedIndex = newIndex;
        }

        // Handle horizontal movement
        float targetXSpeed = 0.0f; // TODO also support arrow keys
        if (IsKeyDown(KEY_A))
        {
            targetXSpeed = -MAX_SPEED;
        }
        else if (IsKeyDown(KEY_D))
        {
            targetXSpeed = MAX_SPEED;
        }

        if (targetXSpeed != 0.0f)
        {
            transform->velocity.x += ACCELERATION * (targetXSpeed > 0.0f ? 1.0f : -1.0f);
            if (transform->velocity.x > MAX_SPEED)
            {
                transform->velocity.x = MAX_SPEED;
            }
            else if (transform->velocity.x < -MAX_SPEED)
            {
                transform->velocity.x = -MAX_SPEED;
            }
        }
        else
        {
            if (transform->velocity.x > ACCELERATION_DECAY)
            {
                transform->velocity.x -= ACCELERATION_DECAY;
            }
            else if (transform->velocity.x < -ACCELERATION_DECAY)
            {
                transform->velocity.x += ACCELERATION_DECAY;
            }
            else
            {
                transform->velocity.x = 0.0f;
            }
        }

        // Handle jump physics
        // Initial press gives burst of velocity, holding it lets the player jump higher
        // Roughly the NES Mario jump
        bool isJumpPressed = IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE);
        bool isJumpHeld = IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE);
        if (isJumpPressed && physics->isGrounded)
        {
            transform->velocity.y = -JUMP_VELOCITY;
        }
        else if (isJumpHeld && (transform->velocity.y < 0.0f))
        {
            transform->velocity.y -= JUMP_VELOCITY_HOLD_BOOST;
        }
    }

    void PlayerSystem::Render()
    {
        // Render hotbar
        auto [hotbar] = world->GetEntityView<HotbarComponent>(playerEntity).value();

        int HOTBAR_SLOTS = HotbarComponent::SLOTS;

        // Get position for centering
        int totalWidth = (HOTBAR_SLOTS * HOTBAR_SLOT_SIZE) + ((HOTBAR_SLOTS - 1) * HOTBAR_SLOT_PADDING);
        int startX = (GetScreenWidth() - totalWidth) / 2;
        int startY = GetScreenHeight() - HOTBAR_SLOT_SIZE - HOTBAR_BOTTOM_MARGIN;

        // Draw slots
        for (int i = 0; i < HOTBAR_SLOTS; i++)
        {
            int x = startX + i * (HOTBAR_SLOT_SIZE + HOTBAR_SLOT_PADDING);
            Color borderColor = (i == hotbar->selectedIndex) ? YELLOW : GRAY;
            DrawRectangle(x, startY, HOTBAR_SLOT_SIZE, HOTBAR_SLOT_SIZE, Color{ 40, 40, 40, 200 });
            DrawRectangleLines(x, startY - 1, HOTBAR_SLOT_SIZE, HOTBAR_SLOT_SIZE, borderColor);
            if (hotbar->slots[i] != BlockType::None)
            {
                BlockDescriptor desc = GetBlockDescriptor(hotbar->slots[i]);
                Texture2D texture = GetHotbarTexture(desc.texturePath);
                DrawTexturePro(
                    texture,
                    Rectangle{ 0.0f, 0.0f, (float)texture.width, (float)texture.height },
                    Rectangle{ (float)x, (float)startY, (float)HOTBAR_SLOT_SIZE, (float)HOTBAR_SLOT_SIZE },
                    Vector2{ 0.0f, 0.0f },
                    0.0f,
                    WHITE
                );
            }
        }
    }

    // TODO share textures with RenderSystem instead
    Texture2D PlayerSystem::GetHotbarTexture(const std::string& path)
    {
        auto it = hotbarTextures.find(path);
        if (it != hotbarTextures.end())
        {
            return it->second;
        }
        Texture2D texture = LoadTexture(path.c_str());
        hotbarTextures.insert({ path, texture });
        return texture;
    }

    PlayerSystem::~PlayerSystem()
    {
        for (auto& [_, texture] : hotbarTextures)
        {
            UnloadTexture(texture);
        }
    }
}
