#pragma once

#include "ComponentType.hpp"

namespace ecs {

    struct CurrencyComponent
    {
        static constexpr ComponentType Type = ComponentType::Currency;

        CurrencyType currencyType = CurrencyType::Coin;
        int amount = 1;
    };

}
