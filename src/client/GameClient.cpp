#include "GameClient.h"
#include "../common/Constants.h"

#include <SDL.h>
#include <cmath>

namespace TankBattle
{
    bool GameClient::init(
        const std::string& host,
        uint16_t port,
        const std::string& name)
    {
        if (!renderer_.init())
            return false;

        if (!net_.connect(host, port))
            return false;

        ClientMessage msg;

        msg.set_type(ClientMessage::JOIN_REQUEST);

        msg.mutable_playerinfo()->set_playername(name);

        net_.send(msg);

        return true;
    }

    void GameClient::processNetwork()
    {
        ServerMessage msg;

        while (net_.pollMessage(msg))
        {
            switch (msg.type())
            {
            case ServerMessage::GAME_STATE:
            {
                currentState_ = msg.gamestate();
                break;
            }

            case ServerMessage::CONNECTION_ACK:
            {
                playerId_ = msg.connectionid();
                break;
            }

            default:
                break;
            }
        }
    }

    void GameClient::handleInput(float)
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running_ = false;
            }
        }

        const Uint8* keys =
            SDL_GetKeyboardState(nullptr);

        input_.set_forward(keys[SDL_SCANCODE_W]);
        input_.set_backward(keys[SDL_SCANCODE_S]);
        input_.set_turnleft(keys[SDL_SCANCODE_A]);
        input_.set_turnright(keys[SDL_SCANCODE_D]);

        int mx, my;

        SDL_GetMouseState(&mx, &my);

        float dx =
            mx - SCREEN_WIDTH * 0.5f;

        float dy =
            my - SCREEN_HEIGHT * 0.5f;

        input_.set_turrettargetangle(
            atan2f(dy, dx));

        Uint32 mouse =
            SDL_GetMouseState(nullptr, nullptr);

        input_.set_shoot(
            mouse & SDL_BUTTON(SDL_BUTTON_LEFT));

        input_.set_playerid(playerId_);

        ClientMessage msg;

        msg.set_type(ClientMessage::PLAYER_INPUT);

        *msg.mutable_input() = input_;

        net_.send(msg);
    }

    void GameClient::run()
    {
        Uint64 prev =
            SDL_GetPerformanceCounter();

        while (running_)
        {
            Uint64 now =
                SDL_GetPerformanceCounter();

            float dt =
                (float)(now - prev) /
                SDL_GetPerformanceFrequency();

            prev = now;

            processNetwork();

            handleInput(dt);

            renderer_.begin();

            renderer_.render(currentState_, playerId_);

            renderer_.end();

            SDL_Delay(1);
        }
    }
}