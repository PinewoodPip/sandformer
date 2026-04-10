#pragma once

#include "Components.hpp"
#include "Events.hpp"
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <tuple>

namespace ecs {

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
        std::map<uint64_t, std::vector<Entity*>> viewCache;
        std::set<uint64_t> dirtyViewComponents;

    public:
        Entity* CreateEntity();

        void Start();
        void Update();
        void Render();
        void AddComponent(Entity* entity, AnyComponent component);

        void AddSystem(System* system)
        {
            systems.push_back(system);
        }

        void EmitEvent(AnyEvent event)
        {
            events.push(event);
        }
    
        template <typename... Ts>
        inline EntityView<Ts...> GetEntities()
        {
            uint64_t bitmask = GetViewKey<Ts...>();
            auto view = viewCache.find(bitmask);
            bool needsRebuild = dirtyViewComponents.find(bitmask) != dirtyViewComponents.end();

            if (view == viewCache.end() || needsRebuild)
            {
                RebuildView<Ts...>();
                view = viewCache.find(bitmask);
            }

            return EntityView<Ts...>{ view->second };
        }

        // TODO iterator
        std::vector<Entity*> GetAllEntities()
        {
            return entities;
        }

    private:
        void ProcessEvents();
        
        template <typename... Ts>
        void RebuildView()
        {
            uint64_t bitmask = GetViewKey<Ts...>();
            std::vector<Entity*> entitiesWithComponents;
            for (Entity* entity : entities)
            {
                if ((entity->GetComponent<Ts>() && ...))
                {
                    entitiesWithComponents.push_back(entity);
                }
            }
            viewCache[bitmask] = entitiesWithComponents;

            dirtyViewComponents.erase(bitmask);
        }

        template <typename... Ts>
        uint64_t GetViewKey()
        {
            return (... | (1ULL << (int)Ts::Type));
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
