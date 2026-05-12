#include "CollisionSystem.h"
#include "../../common/Constants.h"
#include <cmath>


namespace TankBattle
{
    void CollisionSystem::update(World& world)
    {
        for (auto& p : world.projectiles)
        {
            if (!p.isactive()) continue;

            for (auto& [id, player] : world.players)
            {
                auto& tank = player->tankState;

                if (!tank.isalive()) continue;
                if (p.ownerid() == id) continue;

                float dx = p.position().x() - tank.position().x();
                float dy = p.position().y() - tank.position().y();

                if (dx * dx + dy * dy < 400.0f)
                {
                    p.set_isactive(false);

                    int hp = tank.health() - PROJECTILE_DAMAGE;

                    if (hp <= 0)
                    {
                        tank.set_isalive(false);
                        tank.set_health(0);
                    }
                    else
                    {
                        tank.set_health(hp);
                    }
                }
            }
        }

        for (auto it1 = world.players.begin(); it1 != world.players.end(); ++it1)
        {
           auto& tank1 = it1->second->tankState;
           if (!tank1.isalive()) continue;

           for (auto it2 = std::next(it1); it2 != world.players.end(); ++it2)
           {
               auto& tank2 = it2->second->tankState;
               if (!tank2.isalive()) continue;

               float dx = tank1.position().x() - tank2.position().x();
               float dy = tank1.position().y() - tank2.position().y();
               float distSq = dx * dx + dy * dy;
               float minDist = TANK_WIDTH;

               if (distSq < minDist * minDist)
               {
                   float dist = std::sqrt(distSq);
                   float overlap = minDist - dist;
                   float pushX = (dx / dist) * overlap * 0.5f;
                   float pushY = (dy / dist) * overlap * 0.5f;

                   tank1.mutable_position()->set_x(tank1.position().x() + pushX);
                   tank1.mutable_position()->set_y(tank1.position().y() + pushY);
                   tank2.mutable_position()->set_x(tank2.position().x() - pushX);
                   tank2.mutable_position()->set_y(tank2.position().y() - pushY);
               }
           }
        }

        for(auto& p : world.players)
        {
            auto& tank = p.second->tankState;
            if (tank.position().x() < 0.f) {
                tank.mutable_position()->set_x(0.f);
            }
            else if (tank.position().x() > WORLD_SIZE) {
                tank.mutable_position()->set_x(WORLD_SIZE);
            }
            if (tank.position().y() < 0.f) {
                tank.mutable_position()->set_y(0.f);
            }
            else if (tank.position().y() > WORLD_SIZE) {
                tank.mutable_position()->set_y(WORLD_SIZE);
            }
        }
    }
}