#pragma once

#include "Components.hpp"
#include "Events.hpp"
#include "System.hpp"
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <tuple>
#include <optional>
#include <algorithm>
#include <typeindex>
#include <functional>
#include <any>

namespace ecs {

    using ViewKey = std::vector<ComponentType>;

    class Entity;

    template <typename... Ts>
    class EntityView
    {
        std::vector<Entity*>& entities;

    public:

        EntityView(std::vector<Entity*>& entities) : entities(entities) {}

        struct Iterator
        {
            std::vector<Entity*>::iterator it;

            std::tuple<Entity*, Ts*...> operator*()
            {
                return std::tuple_cat(std::make_tuple(*it), std::make_tuple((*it)->GetComponent<Ts>()...));
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

        std::vector<System*> systems;
        std::map<ViewKey, std::vector<Entity*>> viewCache;
        std::set<ViewKey> dirtyViewComponents;

        std::map<std::type_index, std::vector<std::function<void(const std::any&)>>> eventListeners;

    public:
        Entity* CreateEntity();

        void Start();
        void Update();
        void Render();
        void AddComponent(Entity* entity, AnyComponent component);

        void AddSystem(System* system);
        void EmitEvent(events::AnyEvent event);
    
        template <typename... Ts>
        inline EntityView<Ts...> GetEntities()
        {
            const ViewKey& key = GetViewKey<Ts...>();
            auto view = viewCache.find(key);
            bool needsRebuild = dirtyViewComponents.find(key) != dirtyViewComponents.end();

            if (view == viewCache.end() || needsRebuild)
            {
                RebuildEntityView<Ts...>();
                view = viewCache.find(key);
            }

            return EntityView<Ts...>{ view->second };
        }

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
        void ProcessEvents();
        void DestroyEntity(Entity* entity);
        
        template <typename... Ts>
        void RebuildEntityView()
        {
            const ViewKey& key = GetViewKey<Ts...>();
            std::vector<Entity*> entitiesWithComponents;
            for (Entity* entity : entities)
            {
                if ((entity->GetComponent<Ts>() && ...))
                {
                    entitiesWithComponents.push_back(entity);
                }
            }
            viewCache[key] = entitiesWithComponents;

            dirtyViewComponents.erase(key);
        }

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
