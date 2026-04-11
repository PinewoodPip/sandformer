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

    void CurrencySystem::Render()
    {
        // Show the player's coins in top-left
        for (const auto& [entity, player, inventory] : world->GetEntities<PlayerComponent, InventoryComponent>())
        {
            auto it = inventory->currencies.find(CurrencyType::Coin);
            int coins = (it != inventory->currencies.end()) ? it->second : 0; // TODO extract utility getter function
            DrawText(TextFormat("Coins: %d", coins), 10, 10, 20, WHITE);
        }
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
