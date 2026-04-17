#pragma once

#include "raylib.h"
#include "ComponentType.hpp"
#include <string>

namespace ecs {

    struct TextureComponent
    {
        static constexpr ComponentType Type = ComponentType::Texture;

        std::string path;
        Vector2 size = { 32.0f, 32.0f };
    };

}
