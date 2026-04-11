#include "World.hpp"
#include "Entity.hpp"
#include <algorithm>
#include <exception>

namespace ecs {

    Entity* World::CreateEntity()
    {
        Entity* entity = new Entity(_NextEntityID++);
        entities.push_back(entity);

        EmitEvent(EntityCreatedEvent{ entity });

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

        EmitEvent(ComponentAddedEvent{ entity });
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

    void World::ProcessEvents()
    {
        while (!events.empty())
        {
            const AnyEvent& event = events.front();

            // Handle entity destruction requests
            if (auto destroyEntityEvent = std::get_if<RequestDestroyEntityEvent>(&event))
            {
                // Remove from bookkeeping
                entities.erase(std::remove(entities.begin(), entities.end(), destroyEntityEvent->entity), entities.end());

                // Clear all cached views
                // dirtyViewComponents does not need to be updated
                // since this in itself will invalidate all views
                viewCache.clear();

                delete destroyEntityEvent->entity;
            }

            // Forward event
            for (System* system : systems)
            {
                system->ProcessEvent(event);
            }

            events.pop();
        }
    }

}
