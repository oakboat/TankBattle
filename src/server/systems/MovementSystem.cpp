#include "MovementSystem.h"
#include "../Session.h"
#include <cmath>

namespace TankBattle
{
    void MovementSystem::update(World& world, float dt)
    {
        for (auto& [id, player] : world.players)
        {
            auto& tank = player->tankState;
            auto& input = player->input;

            if (!tank.isalive()) continue;

            float rot = tank.rotation();

            if (input.turnleft())
                rot -= TANK_ROTATION_SPEED * dt;

            if (input.turnright())
                rot += TANK_ROTATION_SPEED * dt;

            tank.set_rotation(rot);

            float vx = 0.f;
            float vy = 0.f;

            if (input.forward())
            {
                vx = cos(rot) * TANK_SPEED;
                vy = sin(rot) * TANK_SPEED;
            }

            if (input.backward())
            {
                vx = -cos(rot) * TANK_SPEED * 0.5f;
                vy = -sin(rot) * TANK_SPEED * 0.5f;
            }

            tank.mutable_velocity()->set_x(vx);
            tank.mutable_velocity()->set_y(vy);

            tank.mutable_position()->set_x(
                tank.position().x() + vx * dt);

            tank.mutable_position()->set_y(
                tank.position().y() + vy * dt);

            tank.set_turretrotation(input.turrettargetangle());
        }
    }
}