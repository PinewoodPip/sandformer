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

        void OnStart() override;
        void ProcessEvent(const AnyEvent& event) override;
        void Render() override;

    private:
        std::map<std::string, Image> images; // TODO unload them
        std::map<std::string, Texture2D> textures; // TODO unload them

        Image TryGetImage(std::string path);
        Texture2D TryGetTexture2D(std::string path);
    };
}
