#pragma once

#include "Enums.hpp"
#include <string>

namespace ecs {

    struct BlockDescriptor
    {
        std::string texturePath;
    };

    // TODO define these externally
    inline BlockDescriptor GetBlockDescriptor(BlockType type)
    {
        switch (type)
        {
            case BlockType::Grass: return { "resources/grass.png" };
            case BlockType::Dirt: return { "resources/dirt.png" };
            default: break;
        }
        throw std::exception("Invalid block type");
    }

    // Represents a column & row position on the block grid.
    struct GridPos
    {
        int x;
        int y;
    };

}
