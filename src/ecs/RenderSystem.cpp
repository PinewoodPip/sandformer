#include "RenderSystem.hpp"

namespace ecs
{
    RenderSystem::~RenderSystem()
    {
        // Unload images and textures
        for (auto& [path, image] : images)
        {
            UnloadImage(image);
        }
        for (auto& [path, texture] : textures)
        {
            UnloadTexture(texture);
        }
    }

    void RenderSystem::OnStart() {}

    void RenderSystem::ProcessEvent(const AnyEvent& event) {}

    Image RenderSystem::TryGetImage(std::string path)
    {
        auto it = images.find(path);
        if (it != images.end())
        {
            return it->second;
        }
        else
        {
            Image image = LoadImage(path.c_str());
            images.insert({ path, image });
            auto texture = LoadTextureFromImage(image);
            textures.insert({ path, texture });
            return image;
        }
    }

    Texture2D RenderSystem::TryGetTexture2D(std::string path) // TODO cleanup
    {
        auto it = textures.find(path);
        if (it != textures.end())
        {
            return it->second;
        }
        else
        {
            Image image = TryGetImage(path);
            return LoadTextureFromImage(image);
        }
    }

    void RenderSystem::Render()
    {
        // Draw entities with textures
        // Note: BeginDrawing() and EndDrawing() is handled by the main loop
        for (const auto& [entity, transform, textureComp] : world->GetEntities<TransformComponent, TextureComponent>())
        {
            Texture2D texture = TryGetTexture2D(textureComp->path);
            Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
            Rectangle dest = { transform->position.x, transform->position.y, textureComp->size.x, textureComp->size.y };
            DrawTexturePro(texture, source, dest, Vector2{ 0.0f, 0.0f }, 0.0f, WHITE);
        }
    }
}
