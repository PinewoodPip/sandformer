#pragma once
#include "raylib.h"
#include "Entity.hpp"

namespace ecs
{
    class CurrencySystem : public System
    {
    public:
        CurrencySystem(World* world) : System(world) {}

        void OnStart() override;
        void Render() override;

    private:
        void AddCurrency(InventoryComponent* inventory, CurrencyType type, int amount);
        void PickUpCurrency(InventoryComponent* inventory, Entity* currencyEntity, CurrencyComponent* currency);
    };
}