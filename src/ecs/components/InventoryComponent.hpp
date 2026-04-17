#pragma once

#include "ComponentType.hpp"
#include <unordered_map>

namespace ecs {

    struct InventoryComponent
    {
        static constexpr ComponentType Type = ComponentType::Inventory;

        std::unordered_map<CurrencyType, int> currencies;
    };

}
