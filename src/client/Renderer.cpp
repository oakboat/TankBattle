#include "Renderer.h"

#include "../common/Constants.h"

namespace TankBattle
{
    bool Renderer::init()
    {
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);

        window_ =
            SDL_CreateWindow(
                "TankBattle",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                0);

        renderer_ =
            SDL_CreateRenderer(
                window_,
                -1,
                SDL_RENDERER_ACCELERATED);

        tankBodyTexture_ = loadTexture("assets/tank_body.png");
        tankTurretTexture_ = loadTexture("assets/tank_turret.png");

        SDL_SetRenderTarget(renderer_, tankBodyTexture_);
        SDL_SetRenderDrawColor(renderer_, 0, 200, 0, 255);
        SDL_RenderClear(renderer_);
        SDL_SetRenderTarget(renderer_, nullptr);

        return renderer_ != nullptr;
    }

    SDL_Texture* Renderer::loadTexture(const char* path)
    {
        SDL_Surface* surface = IMG_Load(path);
        if (!surface) {
            printf("Failed to load texture %s: %s\n", path, IMG_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_FreeSurface(surface);

        return texture;
    }

    Renderer::~Renderer()
    {
        if (tankBodyTexture_)
            SDL_DestroyTexture(tankBodyTexture_);

        if (tankTurretTexture_)
            SDL_DestroyTexture(tankTurretTexture_);

        if (renderer_)
            SDL_DestroyRenderer(renderer_);

        if (window_)
            SDL_DestroyWindow(window_);

        SDL_Quit();
    }

    void Renderer::begin()
    {
        SDL_SetRenderDrawColor(
            renderer_,
            30,
            30,
            30,
            255);

        SDL_RenderClear(renderer_);
    }

    void Renderer::drawTank(
        const TankState& tank,
        float camX,
        float camY)
    {
        float x =
            tank.position().x() - camX;

        float y =
            tank.position().y() - camY;

        SDL_Rect dst;
        dst.x = (int)(x - TANK_WIDTH / 2);
        dst.y = (int)(y - TANK_HEIGHT / 2);
        dst.w = TANK_WIDTH;
        dst.h = TANK_HEIGHT;

        SDL_Point center;
        center.x = TANK_WIDTH / 2;
        center.y = TANK_HEIGHT / 2;
        
        SDL_RenderCopyEx(
            renderer_,
            tankBodyTexture_,
            nullptr,
            &dst,
            tank.rotation() * 180.0 / M_PI + 90.0,
            &center,
            SDL_FLIP_NONE);

        float turretX = x;
        float turretY = y;
        SDL_Rect turretDst = dst;

        SDL_RenderCopyEx(
            renderer_,
            tankTurretTexture_,
            nullptr,
            &turretDst,
            tank.turretrotation() * 180.0 / M_PI,
            &center,
            SDL_FLIP_NONE);
    }

    void Renderer::drawProjectile(
        const Projectile& p,
        float camX,
        float camY)
    {
        float x =
            p.position().x() - camX;

        float y =
            p.position().y() - camY;

        SDL_Rect r;

        r.x = (int)x;
        r.y = (int)y;
        r.w = 4;
        r.h = 4;

        SDL_SetRenderDrawColor(
            renderer_,
            255,
            255,
            0,
            255);

        SDL_RenderFillRect(
            renderer_,
            &r);
    }

    void Renderer::render(
        const GameState& state,
        uint32_t localPlayer)
    {
        float camX = 0.f;
        float camY = 0.f;

        for (auto& tank : state.tanks())
        {
            if (tank.id() == localPlayer)
            {
                camX =
                    tank.position().x() -
                    SCREEN_WIDTH * 0.5f;

                camY =
                    tank.position().y() -
                    SCREEN_HEIGHT * 0.5f;
                break;
            }
        }

        for (auto& tank : state.tanks())
        {
            if (tank.isalive())
            {
                drawTank(
                    tank,
                    camX,
                    camY);
            }
        }

        for (auto& p : state.projectiles())
        {
            drawProjectile(
                p,
                camX,
                camY);
        }

        drawBoundary(WORLD_SIZE, WORLD_SIZE, camX, camY);
    }

    void Renderer::drawBoundary(float worldWidth, float worldHeight, float camX, float camY)
    {
        SDL_FRect boundary;
        boundary.x = -camX;
        boundary.y = -camY;
        boundary.w = worldWidth;
        boundary.h = worldHeight;
        
        SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 255);
        SDL_RenderDrawRectF(renderer_, &boundary);
    }

    void Renderer::end()
    {
        SDL_RenderPresent(renderer_);
    }
}