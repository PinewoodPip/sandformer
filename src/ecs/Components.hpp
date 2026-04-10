#pragma once

#include "raylib.h"
#include <map>
#include <string>

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
    };

    enum class CurrencyType : char
    {
        Coin,
    };

    class Component // TODO make structs
    {
    public:
        static constexpr ComponentType Type = ComponentType::None;
        virtual ComponentType GetType() const { return Type; }
    };

    class TransformComponent : public Component
    {
    public:
        static constexpr ComponentType Type = ComponentType::Transform;
        ComponentType GetType() const override { return Type; }

        Vector2 position;

        TransformComponent() : position({ 0, 0 }) {}
        TransformComponent(Vector2 position) : position(position) {}
    };

    class BoundingBoxComponent : public Component
    {
    public:
        static constexpr ComponentType Type = ComponentType::BoundingBox;
        ComponentType GetType() const override { return Type; }

        Vector2 size;
        Vector2 offset; // Offset from Transform position.

        BoundingBoxComponent(Vector2 size, Vector2 offset) : size(size), offset(offset) {}
    };

    class TextureComponent : public Component
    {
    public:
        static constexpr ComponentType Type = ComponentType::Texture;
        ComponentType GetType() const override { return Type; }

        std::string path;
        Vector2 size = { 32, 32 };

        TextureComponent(std::string path) : path(path) {}
        TextureComponent(std::string path, Vector2 size) : path(path), size(size) {}
    };

    class PlayerComponent : public Component
    {
    public:
        static constexpr ComponentType Type = ComponentType::Player;
        ComponentType GetType() const override { return Type; }
    };

    class GravityComponent : public Component
    {
    public:
        static constexpr ComponentType Type = ComponentType::Gravity;
        ComponentType GetType() const override { return Type; }
    };

    class CurrencyComponent : public Component
    {
    public:
        static constexpr ComponentType Type = ComponentType::Currency;
        ComponentType GetType() const override { return Type; }

        CurrencyType currencyType;
        int amount;

        CurrencyComponent(CurrencyType currencyType, int amount = 1) : currencyType(currencyType), amount(amount) {}
    };

    class InventoryComponent : public Component
    {
    public:
        static constexpr ComponentType Type = ComponentType::Inventory;
        ComponentType GetType() const override { return Type; }

        std::map<CurrencyType, int> currencies;
    };

}
