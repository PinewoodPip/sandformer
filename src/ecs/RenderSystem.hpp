#pragma once
#include "raylib.h"
#include "Entity.hpp"
#include <map>
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
        std::map<std::string, Image> images;
        std::map<std::string, Texture2D> textures;

        Image TryGetImage(const std::string& path);
        Texture2D TryGetTexture2D(const std::string& path);
    };
}
