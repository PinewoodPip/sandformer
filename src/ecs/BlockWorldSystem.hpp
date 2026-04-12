#pragma once

#include "System.hpp"

namespace ecs {

    class BlockWorldSystem : public System
    {
    public:
        static constexpr float BLOCK_SIZE = 64.0f;

        using System::System;

        void Update() override;
        void ProcessEvent(const events::AnyEvent& event) override;

    private:
        void BreakBlockAtPos(Vector2 pos);
        bool TryPlaceBlock(BlockType blockType, Vector2 pos);
        void PlaceBlock(BlockType blockType, Vector2 position);
    };
}
