#pragma once

#include "Components.hpp"
#include <variant>

namespace ecs {

    class Entity; // forward declaration

    struct EntityCreatedEvent
    {
        Entity* entity;
    };

    struct ComponentAddedEvent
    {
        Entity* entity;
    };

    struct CollisionEvent
    {
        Entity* entity;
        Entity* otherEntity;
    };

    struct CurrencyPickedUpEvent
    {
        Entity* entity;
        CurrencyType currencyType;
        int amount;
    };

    struct RequestDestroyEntityEvent
    {
        Entity* entity;
    };

    struct RequestBlockCreateEvent
    {
        GridPos pos;
    };

    using AnyEvent = std::variant<EntityCreatedEvent, ComponentAddedEvent, CollisionEvent, CurrencyPickedUpEvent, RequestDestroyEntityEvent, RequestBlockCreateEvent>;

}
