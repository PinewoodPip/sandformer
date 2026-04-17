#pragma once

#include "raylib.h"
#include "ComponentType.hpp"

namespace ecs {

    struct TransformComponent
    {
        static constexpr ComponentType Type = ComponentType::Transform;

        Vector2 position = { 0.0f, 0.0f };
        Vector2 velocity = { 0.0f, 0.0f };
    };

}
