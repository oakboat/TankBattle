#pragma once
#include "../world/World.h"
#include "../../common/Constants.h"

namespace TankBattle
{
    class MovementSystem
    {
    public:
        static void update(World& world, float dt);
    };
}