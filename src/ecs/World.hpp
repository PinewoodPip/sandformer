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

        // TODO iterator
        std::vector<Entity*> GetEntities()
        {
            return entities;
        }

        void AddSystem(System* system)
        {
            systems.push_back(system);
        }

        void Start();
        void Update();
        void Render();
        void ProcessEvents();

        void EmitEvent(AnyEvent event)
        {
            events.push(event);
        }

        void AddComponent(Entity* entity, Component* component);
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
