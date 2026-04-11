#pragma once

#include "System.hpp"

namespace ecs {

    class BlockWorldSystem : public System
    {
    public:
        static constexpr int BLOCK_SIZE = 64;

        using System::System;

        void Update() override;

    private:
        void BreakBlockAtPos(Vector2 pos);
        void PlaceBlockAtPos(Vector2 pos);
        void PlaceBlock(Vector2 position);
    };
}
