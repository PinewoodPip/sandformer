#include "World.hpp"
#include "Entity.hpp"
#include <algorithm>
#include <exception>

namespace ecs {

    Entity* World::CreateEntity()
    {
        Entity* entity = new Entity();
        entity->ID = _NextEntityID++;
        entities.push_back(entity);

        EmitEvent(EntityCreatedEvent{ entity });

        return entity;
    }

    void World::AddComponent(Entity* entity, Component* component)
    {
        ComponentType componentType = component->GetType();
        entity->components.insert({ componentType, component });

        EmitEvent(ComponentAddedEvent{ entity, component });
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
                // Destroy entity & its components
                for (auto& [_, component] : destroyEntityEvent->entity->components)
                {
                    delete component;
                }
                delete destroyEntityEvent->entity;

                // Remove from bookkeeping
                entities.erase(std::remove(entities.begin(), entities.end(), destroyEntityEvent->entity), entities.end());
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
