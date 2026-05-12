// ShootingSystem.h
#pragma once
#include "../world/World.h"

namespace TankBattle
{
    class ShootingSystem
    {
    public:
        static void update(World &world, float dt);
    };
}