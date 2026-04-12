#include "CurrencySystem.hpp"

using namespace ecs::events;

namespace ecs
{
    void CurrencySystem::OnStart()
    {
        // Handle picking up currency
        world->RegisterListener<Collision>([this](const Collision& collision)
        {
            auto* inventoryEntity = collision.entity;
            auto* currencyEntity = collision.otherEntity;
            auto inventoryView = world->GetEntityView<InventoryComponent>(inventoryEntity);
            if (!inventoryView.has_value())
            {
                // Swap entities so that the inventory holder is first
                // Having a collision between 2 currency entities makes one take priority, yep - TODO?
                std::swap(inventoryEntity, currencyEntity);
                inventoryView = world->GetEntityView<InventoryComponent>(inventoryEntity);
            }

            if (inventoryView.has_value())
            {
                auto [inventory] = inventoryView.value();
                auto* currency = currencyEntity->GetComponent<CurrencyComponent>();
                if (currency)
                {
                    PickUpCurrency(inventory, currencyEntity, currency);
                }
            }
        });
    }

    void CurrencySystem::AddCurrency(InventoryComponent* inventory, CurrencyType type, int amount)
    {
        inventory->currencies[type] += amount;
    }

    void CurrencySystem::PickUpCurrency(InventoryComponent* inventory, Entity* currencyEntity, CurrencyComponent* currency)
    {
        AddCurrency(inventory, currency->currencyType, currency->amount);
        world->EmitEvent(CurrencyPickedUp{ currencyEntity, currency->currencyType, currency->amount });
        world->EmitEvent(RequestDestroyEntity{ currencyEntity });
    }

    void CurrencySystem::Render()
    {
        // Show the player's coins in top-left
        auto [_, player, inventory] = world->GetEntity<PlayerComponent, InventoryComponent>().value();
        auto it = inventory->currencies.find(CurrencyType::Coin);
        int coins = (it != inventory->currencies.end()) ? it->second : 0; // TODO extract utility getter function
        DrawText(TextFormat("Coins: %d", coins), 10, 10, 20, WHITE);
    }
}
