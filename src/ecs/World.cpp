#include "World.hpp"
#include "Entity.hpp"
#include <algorithm>
#include <exception>

using namespace ecs::events;

namespace ecs {

    void World::AddSystem(System* system)
    {
        if (started)
        {
            throw std::exception("Cannot add system after ECS world has started");
        }
        systems.push_back(system);
    }

    void World::PushEvent(AnyEvent event)
    {
        events.push(event);
    }

    Entity* World::CreateEntity()
    {
        Entity* entity = new Entity(nextEntityId++);
        entities.push_back(entity);

        PushEvent(EntityCreated{ entity });

        // Note: the new entity does not need to be added to any view until it receives a component
        // since 0-component queries are not allowed
        return entity;
    }

    void World::AddComponent(Entity* entity, AnyComponent component)
    {
        ComponentType type = std::visit([](const auto& c) -> ComponentType { return c.Type; }, component);
        entity->components.insert({ type, std::move(component) });

        AddEntityToViews(entity, type);

        PushEvent(ComponentAdded{ entity });
    }

    void World::AddEntityToViews(Entity* entity, ComponentType addedType)
    {
        for (auto& [viewKey, viewEntities] : viewCache)
        {
            if (std::find(viewKey.begin(), viewKey.end(), addedType) == viewKey.end())
            {
                continue;
            }

            // Add the entity to the view if it now has all its components
            bool hasAllComponents = true;
            for (ComponentType ct : viewKey)
            {
                if (entity->components.find(ct) == entity->components.end())
                {
                    hasAllComponents = false;
                    break;
                }
            }
            if (hasAllComponents)
            {
                viewEntities.insert(entity);
            }
        }
    }

    void World::RemoveEntityFromViews(Entity* entity)
    {
        for (auto& [viewKey, viewEntities] : viewCache)
        {
            viewEntities.erase(entity);
        }
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

        RemoveEntityFromViews(entity);

        delete entity;
    }
}
