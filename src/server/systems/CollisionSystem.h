#pragma once
#include "../world/World.h"
#include "../../common/Constants.h"

namespace TankBattle
{
    class CollisionSystem
    {
    public:
        static void update(World& world);
    };
}