#include "CurrencySystem.hpp"

namespace ecs
{
    void CurrencySystem::AddCurrency(InventoryComponent* inventory, CurrencyType type, int amount)
    {
        inventory->currencies[type] += amount;
    }

    void CurrencySystem::PickUpCurrency(InventoryComponent* inventory, Entity* currencyEntity, CurrencyComponent* currency)
    {
        AddCurrency(inventory, currency->currencyType, currency->amount);
        world->EmitEvent(CurrencyPickedUpEvent{ currencyEntity, currency->currencyType, currency->amount });
        world->EmitEvent(RequestDestroyEntityEvent{ currencyEntity });
    }

    void CurrencySystem::ProcessEvent(const AnyEvent& event)
    {
        if (const auto* e = std::get_if<CollisionEvent>(&event))
        {
            auto* entityA = e->entity;
            auto* entityB = e->otherEntity;
            auto viewA = world->GetEntityView<InventoryComponent>(entityA);
            if (!viewA.has_value())
            {
                // Swap entities so that A is the one with the inventory
                // Having a collision between 2 currency entities makes one take priority, yep - TODO?
                std::swap(entityA, entityB);
                viewA = world->GetEntityView<InventoryComponent>(entityA);
            }

            if (viewA.has_value())
            {
                auto [inventory] = viewA.value();
                auto* currency = entityB->GetComponent<CurrencyComponent>();
                if (currency)
                {
                    PickUpCurrency(inventory, entityB, currency);
                }
            }
        }
    }
}
