#pragma once

#include "Components.hpp"
#include "Events.hpp"
#include "World.hpp"
#include <unordered_map>

namespace ecs {

    class Entity
    {
    public:
        const int ID = 0;

		Entity(int id) : ID(id) {}

        template <typename T>
        T* GetComponent()
        {
            auto it = components.find(T::Type);
            if (it != components.end())
            {
                return std::get_if<T>(&it->second);
            }
            return nullptr;
        }

	private:
        std::unordered_map<ComponentType, AnyComponent> components;

		friend class World;
    };

}
