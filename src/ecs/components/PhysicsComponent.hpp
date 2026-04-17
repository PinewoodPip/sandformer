#pragma once

#include "ComponentType.hpp"

namespace ecs {

    struct PhysicsComponent
    {
        static constexpr ComponentType Type = ComponentType::Gravity;

        bool isGrounded = false;
    };

}
