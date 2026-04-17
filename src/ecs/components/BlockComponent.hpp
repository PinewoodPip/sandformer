#pragma once

#include "ComponentType.hpp"

namespace ecs {

    struct BlockComponent
    {
        static constexpr ComponentType Type = ComponentType::Block;

        BlockType blockType = BlockType::Grass;
    };

}
