#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.pb.h"

namespace TankBattle
{
    class Renderer
    {
    public:
        bool init();

        SDL_Texture* loadTexture(const char* path);

        ~Renderer();

        void begin();

        void render(
            const GameState& state,
            uint32_t localPlayer);

        void end();

    private:
        void drawTank(
            const TankState& tank,
            float camX,
            float camY);

        void drawProjectile(
            const Projectile& p,
            float camX,
            float camY);
        
        void drawBoundary(
            float worldWidth,
            float worldHeight,
            float camX,
            float camY);

    private:
        SDL_Window* window_ = nullptr;

        SDL_Renderer* renderer_ = nullptr;

        SDL_Texture* tankBodyTexture_;
        SDL_Texture* tankTurretTexture_;
    };
}