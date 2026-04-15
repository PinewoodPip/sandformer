#pragma once

#include "Components.hpp"
#include "Events.hpp"
#include "System.hpp"
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <optional>
#include <algorithm>
#include <typeindex>
#include <functional>
#include <any>

namespace ecs {

    using ViewKey = std::vector<ComponentType>; // Note: could've been a bitfield, but we expect >64 components eventually

    class Entity;

    // A view into a cache of all entities that have all the given components.
    template <typename... Ts>
    class EntityView
    {
        std::unordered_set<Entity*>& entities;

    public:

        EntityView(std::unordered_set<Entity*>& entities) : entities(entities) {}

        struct Iterator
        {
            std::unordered_set<Entity*>::iterator it;

            std::tuple<Entity&, Ts*...> operator*()
            {
                return std::tuple<Entity&, Ts*...>{ **it, (*it)->GetComponent<Ts>()... };
            }

            Iterator& operator++()
            {
                ++it;
                return *this;
            }

            bool operator!=(const Iterator& other) const
            {
                return it != other.it;
            }

            bool operator==(const Iterator& other) const
            {
                return it == other.it;
            }
        };

        Iterator begin()
        {
            return Iterator{ entities.begin() };
        }

        Iterator end()
        {
            return Iterator{ entities.end() };
        }
    };

    class World
    {
    private:
        std::vector<Entity*> entities;
        std::queue<events::AnyEvent> events;
        int nextEntityId = 0;
        bool started = false;

        std::vector<System*> systems; // In order of ticking.
        std::map<ViewKey, std::unordered_set<Entity*>> viewCache;

        std::unordered_map<std::type_index, std::vector<std::function<void(const std::any&)>>> eventListeners;

    public:
        // Calls OnStart() on registered systems.
        // Should only be called once after all systems were registered.
        void Start();

        // Processes queued events and calls Update() on systems.
        void Update();

        // Calls Render() on registered systems.
        // raylib BeginDrawing() and EndDrawing() should be called before and after.
        void Render();

        // Creates a new entity with no components.
        Entity* CreateEntity();

        // Adds a component to an entity.
        void AddComponent(Entity* entity, AnyComponent component);

        // Registers a system.
        // The newly-registered system will be ticked after all other previously-registered ones.
        // Should be called before Start().
        // The world will take ownership of the system.
        void AddSystem(System* system);

        // Queues an event to be dispatched to systems on the next tick.
        void PushEvent(events::AnyEvent event);
    
        // Returns all entities with the given components.
        template <typename... Ts>
        inline EntityView<Ts...> GetEntities()
        {
            static_assert(sizeof...(Ts) > 0, "GetEntities(): must query at least one component type"); // Empty query would just be equivalent to iterating all entities; no point in "caching" that

            const ViewKey& key = GetViewKey<Ts...>();
            auto view = viewCache.find(key);

            if (view == viewCache.end())
            {
                RebuildEntityView<Ts...>();
                view = viewCache.find(key);
            }

            return EntityView<Ts...>{ view->second };
        }

        // Returns the first entity with the given components, if any.
        template <typename... Ts>
        inline std::optional<std::tuple<Entity&, Ts*...>> GetEntity()
        {
            auto view = GetEntities<Ts...>();
            auto it = view.begin();
            if (it == view.end())
            {
                return std::nullopt;
            }
            return *it;
        }

        // Returns a view into the entity's given components.
        // Will be valueless if the entity does not have *all* of the requested components.
        template <typename... Ts>
        inline std::optional<std::tuple<Ts*...>> GetEntityView(Entity* entity)
        {
            bool isValid = ((... && entity->GetComponent<Ts>()));
            return isValid ? std::make_optional(std::make_tuple(entity->GetComponent<Ts>()...)) : std::nullopt;
        }

        // Subscribes to an event.
        // TODO an unsubscription mechanism, just in case we ever have system destruction/removal
        // Could be automatic with a RAII handle pattern
        template <typename EventType>
        inline void RegisterListener(std::function<void(const EventType&)> listener)
        {
            auto typeIndex = std::type_index(typeid(EventType));
            if (eventListeners.find(typeIndex) == eventListeners.end())
            {
                eventListeners[typeIndex] = {};
            }
            eventListeners[typeIndex].push_back([listener](const std::any& event)
            {
                listener(std::any_cast<const EventType&>(event));
            });
        }

        ~World();

    private:
        // Forwards all queued events to the subscribed systems.
        void ProcessEvents();

        // Removes an entity and all its components.
        void DestroyEntity(Entity* entity);
        
        template <typename... Ts>
        void RebuildEntityView()
        {
            const ViewKey& key = GetViewKey<Ts...>();
            std::unordered_set<Entity*> entitiesWithComponents;
            for (Entity* entity : entities)
            {
                if ((entity->GetComponent<Ts>() && ...))
                {
                    entitiesWithComponents.insert(entity);
                }
            }
            viewCache[key] = std::move(entitiesWithComponents);
        }

        // Adds an entity to every cached view that it would belong to after adding the given component.
        void AddEntityToViews(Entity* entity, ComponentType addedCompType);

        // Removes an entity from all cached views.
        void RemoveEntityFromViews(Entity* entity);

        // Dispatches a concrete event to its subscribed listeners.
        template <typename EventType>
        inline void ProcessEvent(const EventType& event)
        {
            auto typeIndex = std::type_index(typeid(EventType));
            auto listeners = eventListeners.find(typeIndex);
            if (listeners != eventListeners.end())
            {
                for (const auto& listener : listeners->second)
                {
                    listener(event);
                }
            }
        }

        // Returns the ViewKey for a set of component types.
        template <typename... Ts>
        const ViewKey& GetViewKey()
        {
            // Will only be initialized once per combination of component types, thus the sort only runs once per combo
            static const ViewKey key = []()
            {
                ViewKey k = { Ts::Type... };
                std::sort(k.begin(), k.end());
                return k;
            }();
            return key;
        }
    };
}
