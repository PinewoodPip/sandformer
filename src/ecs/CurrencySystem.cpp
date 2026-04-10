#include "CurrencySystem.hpp"

namespace ecs
{
    void ecs::CurrencySystem::ProcessEvent(const AnyEvent& event)
    {
        if (const auto* e = std::get_if<CollisionEvent>(&event))
        {
            auto* entityA = e->entity;
            auto* entityB = e->otherEntity;
            InventoryComponent* inventory = entityA->GetComponent<InventoryComponent>();
            if (!inventory)
            {
                // Swap entities so that A is the one with the inventory
                std::swap(entityA, entityB);
                inventory = entityA->GetComponent<InventoryComponent>();
            }
            auto* currency = entityB->GetComponent<CurrencyComponent>();
            if (inventory && currency)
            {
                PickUpCurrency(inventory, entityB, currency);
            }
        }
    }
}
