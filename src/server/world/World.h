#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "game.pb.h"

namespace TankBattle
{
    class Session;
    class Player
    {
    public:
        uint32_t id;
        std::string playerName;
        PlayerInput input;
        TankState tankState;
        float lastShootTime = 0.f;
        
        Player(uint32_t id, const std::string& name) : id(id), playerName(name) {
            tankState.set_id(id);
            tankState.set_isalive(true);
            tankState.set_health(100);
            tankState.set_playername(name);
        }

        void resetShootCooldown() {
            lastShootTime = 0.f;
        }
        void updateCooldown(float dt) {
            lastShootTime += dt;
        }
        bool canShoot() const {
            return lastShootTime >= 0.1f; // 假设射击冷却为1秒
        }
        
    };

    struct World
    {
        uint32_t nextPlayerId_ = 1;
        uint32_t nextProjectileId_ = 1;
        std::unordered_map<uint32_t, std::shared_ptr<Session>> sessions;
        std::unordered_map<uint32_t, std::shared_ptr<Player>> players;
        std::vector<Projectile> projectiles;
    };
}