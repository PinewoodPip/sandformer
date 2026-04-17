#pragma once

namespace ecs {

    enum class ComponentType : int
    {
        None = -1,
        Transform,
        BoundingBox,
        Texture,
        Player,
        Gravity,
        Currency,
        Inventory,
        Solid,
        Block,
        Hotbar,
    };

    enum class BlockType
    {
        None,
        Grass,
        Dirt,
    };

    enum class CurrencyType : char
    {
        Coin,
    };

}
