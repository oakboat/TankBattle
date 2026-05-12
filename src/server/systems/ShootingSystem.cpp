// ShootingSystem.cpp
#include "ShootingSystem.h"
#include "../Session.h"
#include <cmath>

namespace TankBattle
{
    void ShootingSystem::update(World& world, float dt)
    {
        for (auto& [id, player] : world.players)
        {
            auto& input = player->input;
            auto& tank = player->tankState;
            
            if (!tank.isalive()) continue;
            
            // 检查射击输入和冷却时间
            if (input.shoot() && player->canShoot())
            {
                // 创建新子弹
                Projectile projectile;
                projectile.set_id(world.nextProjectileId_++);
                projectile.set_ownerid(id);
                projectile.set_isactive(true);
                
                // 设置子弹起始位置（坦克炮口）
                float angle = tank.turretrotation();
                float offset = 30.0f;  // 炮口偏移距离
                auto* pos = projectile.mutable_position();
                pos->set_x(tank.position().x() + cos(angle) * offset);
                pos->set_y(tank.position().y() + sin(angle) * offset);
                
                // 设置子弹速度
                float bulletSpeed = 500.0f;
                auto* vel = projectile.mutable_velocity();
                vel->set_x(cos(angle) * bulletSpeed);
                vel->set_y(sin(angle) * bulletSpeed);
                
                // 设置旋转角度
                projectile.set_rotation(angle);
                
                world.projectiles.push_back(projectile);

                player->resetShootCooldown();
            }

            player->updateCooldown(dt);
        }
    }
}