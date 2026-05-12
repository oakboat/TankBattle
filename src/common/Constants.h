#pragma once

#include <cstdint>
#include <cmath>

namespace TankBattle
{
    // =========================
    // Screen
    // =========================
    constexpr int SCREEN_WIDTH  = 1280;
    constexpr int SCREEN_HEIGHT = 720;

    // =========================
    // World
    // =========================
    constexpr float WORLD_SIZE = 5000.0f;

    // =========================
    // Tank
    // =========================
    constexpr int TANK_WIDTH  = 40;
    constexpr int TANK_HEIGHT = 40;

    constexpr float TANK_SPEED = 200.0f;              // units/sec
    constexpr float TANK_ROTATION_SPEED = 2.5f;      // rad/sec
    constexpr float MAX_VELOCITY = 300.0f;

    constexpr int TANK_MAX_HEALTH = 100;

    constexpr float COLLISION_RADIUS = 20.0f;

    // =========================
    // Projectile
    // =========================
    constexpr float PROJECTILE_SPEED = 500.0f;
    constexpr int PROJECTILE_SIZE = 6;

    constexpr float PROJECTILE_COLLISION_RADIUS = 4.0f;

    constexpr int PROJECTILE_DAMAGE = 25;
    constexpr float PROJECTILE_COOLDOWN = 0.25f; // seconds

    // =========================
    // Network
    // =========================
    constexpr int MAX_MESSAGE_SIZE = 64 * 1024;

    constexpr int TICK_RATE = 60;
    constexpr int CLIENT_UPDATE_RATE = 60;

    // =========================
    // Math helpers
    // =========================
    constexpr float PI = 3.14159265358979323846f;
    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;
}