#pragma once

#include "NetClient.h"
#include "Renderer.h"

namespace TankBattle
{
    class GameClient
    {
    public:
        bool init(
            const std::string& host,
            uint16_t port,
            const std::string& name);

        void run();

    private:
        void handleInput(float dt);

        void processNetwork();

    private:
        NetClient net_;

        Renderer renderer_;

        GameState currentState_;

        uint32_t playerId_ = 0;

        bool running_ = true;

        PlayerInput input_;
    };
}