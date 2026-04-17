#pragma once

#include "raylib.h"
#include "ComponentType.hpp"

namespace ecs {

    struct BoundingBoxComponent
    {
        static constexpr ComponentType Type = ComponentType::BoundingBox;

        Vector2 size = { 0.0f, 0.0f };
        Vector2 offset = { 0.0f, 0.0f };
    };

}
