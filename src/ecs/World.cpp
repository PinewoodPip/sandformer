#include "World.hpp"
#include "Entity.hpp"
#include <algorithm>
#include <exception>

using namespace ecs::events;

namespace ecs {

    void World::AddSystem(System* system)
    {
        systems.push_back(system);
    }

    void World::EmitEvent(AnyEvent event)
    {
        events.push(event);
    }

    Entity* World::CreateEntity()
    {
        Entity* entity = new Entity(nextEntityId++);
        entities.push_back(entity);

        EmitEvent(EntityCreated{ entity });

        return entity;
    }

    void World::AddComponent(Entity* entity, AnyComponent component)
    {
        ComponentType type = std::visit([](const auto& c) -> ComponentType { return c.Type; }, component);
        entity->components.insert({ type, std::move(component) });

        // Mark views using this component type as dirty
        for (const auto& [viewKey, viewEntities] : viewCache)
        {
            if (std::find(viewKey.begin(), viewKey.end(), type) != viewKey.end())
            {
                dirtyViewComponents.insert(viewKey);
            }
        }

        EmitEvent(ComponentAdded{ entity });
    }

    void World::Start()
    {
        if (started)
        {
            throw std::exception("ECS world already started");
        }
        for (System* system : systems)
        {
            system->OnStart();
        }
        started = true;
    }

    void World::Update()
    {
        // Process events queued from last tick
        ProcessEvents();

        for (System* system : systems)
        {
            system->Update();
        }
    }

    void World::Render()
    {
        for (System* system : systems)
        {
            system->Render();
        }
    }

    World::~World()
    {
        // Cleanup entities
        // Note: does not use DestroyEntity() as that would inefficiently shift the vector for each entity (and modify this iterator)
        for (Entity* entity : entities)
        {
            delete entity;
        }

        // Cleanup systems
        for (System* system : systems)
        {
            delete system;
        }
    }

    void World::ProcessEvents()
    {
        while (!events.empty())
        {
            const AnyEvent& event = events.front();

            // Handle entity destruction requests
            if (auto destroyEntityEvent = std::get_if<RequestDestroyEntity>(&event))
            {
                DestroyEntity(destroyEntityEvent->entity);
            }

            // Forward event
            // Must extract the actual type from the variant
            std::visit([this](const auto& e)
            {
                ProcessEvent(e);
            }, event);

            events.pop();
        }
    }

    void World::DestroyEntity(Entity* entity)
    {
        // Remove from bookkeeping
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());

        // Clear all cached views
        // dirtyViewComponents does not need to be updated
        // since this in itself will invalidate all views
        viewCache.clear();

        delete entity;
    }
}
