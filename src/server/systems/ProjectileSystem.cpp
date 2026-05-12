#include "ProjectileSystem.h"
#include <algorithm>

namespace TankBattle
{
    void ProjectileSystem::update(World& world, float dt)
    {
        for (auto& p : world.projectiles)
        {
            if (!p.isactive()) continue;
            auto* pos = p.mutable_position();
            auto* vel = p.mutable_velocity();

            pos->set_x(pos->x() + vel->x() * dt);
            pos->set_y(pos->y() + vel->y() * dt);

            if (pos->x() < 0 || pos->x() > WORLD_SIZE ||
                pos->y() < 0 || pos->y() > WORLD_SIZE)
            {
                p.set_isactive(false);
            }
        }

        world.projectiles.erase(
            std::remove_if(world.projectiles.begin(),
                           world.projectiles.end(),
                           [](const Projectile& p)
                           {
                               return !p.isactive();
                           }),
            world.projectiles.end());
    }
}