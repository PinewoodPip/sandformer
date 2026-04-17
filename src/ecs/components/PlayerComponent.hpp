#pragma once

#include "ComponentType.hpp"

namespace ecs {

    struct PlayerComponent
    {
        static constexpr ComponentType Type = ComponentType::Player;
    };

}
