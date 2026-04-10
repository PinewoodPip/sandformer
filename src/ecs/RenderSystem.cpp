#include "RenderSystem.hpp"

namespace ecs
{
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
            DrawTexture(texture, transform->position.x, transform->position.y, WHITE); // TODO use scale from the component
        }
    }
}
