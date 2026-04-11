#pragma once

#include "Components.hpp"
#include "Events.hpp"
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <tuple>
#include <optional>
#include <algorithm>

namespace ecs {

    using ViewKey = std::vector<ComponentType>;

    class Entity;
    class System;

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
        std::queue<AnyEvent> events;
        int _NextEntityID = 0;
        bool started = false;

        std::vector<System*> systems;
        std::map<ViewKey, std::vector<Entity*>> viewCache;
        std::set<ViewKey> dirtyViewComponents;

    public:
        Entity* CreateEntity();

        void Start();
        void Update();
        void Render();
        void AddComponent(Entity* entity, AnyComponent component);

        void AddSystem(System* system);
        void EmitEvent(AnyEvent event);
    
        template <typename... Ts>
        inline EntityView<Ts...> GetEntities()
        {
            const ViewKey& key = GetViewKey<Ts...>();
            auto view = viewCache.find(key);
            bool needsRebuild = dirtyViewComponents.find(key) != dirtyViewComponents.end();

            if (view == viewCache.end() || needsRebuild)
            {
                RebuildView<Ts...>();
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

    private:
        void ProcessEvents();
        
        template <typename... Ts>
        void RebuildView()
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

    class System
    {
    public:
        virtual void OnStart() {};
        virtual void ProcessEvent(const AnyEvent& event) {};
        virtual void Update() {};
        virtual void Render() {};

        System(World* world) : world(world) {}

    protected:
        World* world;
    };
}
