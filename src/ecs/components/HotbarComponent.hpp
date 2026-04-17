#pragma once

#include "ComponentType.hpp"
#include <array>

namespace ecs {

    struct HotbarComponent
    {
        static constexpr ComponentType Type = ComponentType::Hotbar;
        static constexpr int SLOTS = 6;

        std::array<BlockType, SLOTS> slots; // Slots will be initialized to None since that's the first ID
        int selectedIndex = 0;
    };

}
