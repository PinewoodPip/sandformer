#pragma once

#include "ComponentType.hpp"

namespace ecs {

    struct SolidComponent
    {
        static constexpr ComponentType Type = ComponentType::Solid;
    };

}
