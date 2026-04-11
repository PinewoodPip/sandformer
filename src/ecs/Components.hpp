#pragma once

#include "raylib.h"
#include <map>
#include <string>
#include <variant>

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
        Destructible,
    };

    // Represents a column & row position on the block grid.
    struct GridPos
    {
        int x;
        int y;
    };

    enum class CurrencyType : char
    {
        Coin,
    };

    struct TransformComponent
    {
        static constexpr ComponentType Type = ComponentType::Transform;

        Vector2 position = { 0.0f, 0.0f };
        Vector2 velocity = { 0.0f, 0.0f };
    };

    struct BoundingBoxComponent
    {
        static constexpr ComponentType Type = ComponentType::BoundingBox;

        Vector2 size = { 0.0f, 0.0f };
        Vector2 offset = { 0.0f, 0.0f };
    };

    struct TextureComponent
    {
        static constexpr ComponentType Type = ComponentType::Texture;

        std::string path;
        Vector2 size = { 32.0f, 32.0f };
    };

    struct PlayerComponent
    {
        static constexpr ComponentType Type = ComponentType::Player;
    };

    struct PhysicsComponent
    {
        static constexpr ComponentType Type = ComponentType::Gravity;

        bool isGrounded;
    };

    struct CurrencyComponent
    {
        static constexpr ComponentType Type = ComponentType::Currency;

        CurrencyType currencyType = CurrencyType::Coin;
        int amount = 1;
    };

    struct InventoryComponent
    {
        static constexpr ComponentType Type = ComponentType::Inventory;

        std::map<CurrencyType, int> currencies;
    };

    struct SolidComponent
    {
        static constexpr ComponentType Type = ComponentType::Solid;
    };

    struct DestructibleComponent
    {
        static constexpr ComponentType Type = ComponentType::Destructible;
    };

    using AnyComponent = std::variant<
        TransformComponent,
        BoundingBoxComponent,
        TextureComponent,
        PlayerComponent,
        PhysicsComponent,
        CurrencyComponent,
        InventoryComponent,
        SolidComponent,
        DestructibleComponent
    >;
}
