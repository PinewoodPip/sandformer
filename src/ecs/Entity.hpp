#pragma once

#include "Components.hpp"
#include "Events.hpp"
#include "World.hpp"
#include <map>

namespace ecs {

    class Entity
    {
    public:
        int ID;
        std::map<ComponentType, Component*> components;

        template <typename T>
        T* GetComponent()
        {
            ComponentType componentType = T::Type;
            auto component = components.find(componentType);
            if (component != components.end())
            {
                return static_cast<T*>(component->second);
            }
            else
            {
                return nullptr;
            }
        }
    };

}
