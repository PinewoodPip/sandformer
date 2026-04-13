#pragma once
#include "raylib.h"
#include "Entity.hpp"
#include <unordered_map>
#include <string>

namespace ecs
{
    class RenderSystem : public System
    {
    public:
        RenderSystem(World* world) : System(world) {}
        ~RenderSystem();

        void Render() override;

    private:
        std::unordered_map<std::string, Image> images;
        std::unordered_map<std::string, Texture2D> textures;

        Image TryGetImage(const std::string& path);
        Texture2D TryGetTexture2D(const std::string& path);
    };
}
