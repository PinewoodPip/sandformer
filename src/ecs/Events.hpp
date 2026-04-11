#pragma once

#include "Components.hpp"
#include <variant>

namespace ecs {

    class Entity; // forward declaration

    namespace events {

        struct EntityCreated
        {
            Entity* entity;
        };

        struct ComponentAdded
        {
            Entity* entity;
        };

        struct Collision
        {
            Entity* entity;
            Entity* otherEntity;
        };

        struct CurrencyPickedUp
        {
            Entity* entity;
            CurrencyType currencyType;
            int amount;
        };

        struct RequestDestroyEntity
        {
            Entity* entity;
        };

        struct RequestCreateBlock
        {
            GridPos pos;
            BlockType blockType = BlockType::Grass;
        };

        using AnyEvent = std::variant<EntityCreated, ComponentAdded, Collision, CurrencyPickedUp, RequestDestroyEntity, RequestCreateBlock>;

    }
}
