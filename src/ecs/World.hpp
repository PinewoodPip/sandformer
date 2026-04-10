#pragma once

#include "Components.hpp"
#include "Events.hpp"
#include <vector>
#include <queue>

namespace ecs {

    class Entity;
    class System;

    class World
    {
    private:
        std::vector<Entity*> entities;
        std::queue<AnyEvent> events;
        int _NextEntityID = 0;
        bool started = false;

        std::vector<System*> systems;

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

        // TODO iterator
        std::vector<Entity*> GetEntities()
        {
            return entities;
        }

    private:
        void ProcessEvents();
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
