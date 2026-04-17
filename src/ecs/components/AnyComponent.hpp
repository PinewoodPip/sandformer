#pragma once

#include "TransformComponent.hpp"
#include "BoundingBoxComponent.hpp"
#include "TextureComponent.hpp"
#include "PlayerComponent.hpp"
#include "PhysicsComponent.hpp"
#include "CurrencyComponent.hpp"
#include "InventoryComponent.hpp"
#include "SolidComponent.hpp"
#include "BlockComponent.hpp"
#include "HotbarComponent.hpp"
#include <variant>

namespace ecs {

    // TODO this is poorly maintainable and causes long recompiles if modified - should switch back to using dynamic_cast instead, the performance hit would be minimal
    using AnyComponent = std::variant<
        TransformComponent,
        BoundingBoxComponent,
        TextureComponent,
        PlayerComponent,
        PhysicsComponent,
        CurrencyComponent,
        InventoryComponent,
        SolidComponent,
        BlockComponent,
        HotbarComponent
    >;

}
