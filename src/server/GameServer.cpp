#include "GameServer.h"
#include "Session.h"
#include "world/World.h"
#include <iostream>
#include "systems/CollisionSystem.h"
#include "systems/MovementSystem.h"
#include "systems/ProjectileSystem.h"
#include "systems/ShootingSystem.h"

namespace TankBattle
{
    GameServer::GameServer(uint16_t port)
        : acceptor_(
            io_,
            boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(),
                port)),
        tickTimer_(io_)
    {
        startAccept();
        updateTick();

        std::cout << "Server started\n";
    }

    void GameServer::startAccept()
    {
        auto session =
            std::make_shared<Session>(
                io_,
                this,
                world_.nextPlayerId_++);
        
        acceptor_.async_accept(
            session->socket,
            [this, session](boost::system::error_code ec)
            {
                if (!ec)
                {
                    world_.sessions[session->id] = session;

                    std::cout
                        << "Player connected: "
                        << session->id
                        << std::endl;

                    session->start();
                }

                startAccept();
            });
    }

    void GameServer::disconnect(uint32_t id)
    {
        auto it = world_.sessions.find(id);

        if (it == world_.sessions.end())
            return;

        boost::system::error_code ec;

        it->second->socket.close(ec);

        world_.sessions.erase(it);
        
        auto playerIt = world_.players.find(id);
        if (playerIt != world_.players.end())
        {
            world_.players.erase(playerIt);
        }

        std::cout
            << "Player disconnected: "
            << id
            << std::endl;
    }

    void GameServer::handleClientMessage(
        uint32_t playerId,
        const ClientMessage& msg)
    {
        auto it = world_.sessions.find(playerId);

        if (it == world_.sessions.end())
            return;

        auto& session = it->second;

        switch (msg.type())
        {
        case ClientMessage::JOIN_REQUEST:
        {
            auto player = std::make_shared<Player>(session->id, msg.playerinfo().playername());
            auto position = player->tankState.mutable_position();
            position->set_x(100.f + (player->id - 1) * 100.f);
            position->set_y(100.f);
            world_.players[session->id] = player;

            ServerMessage ack;
            ack.set_type(ServerMessage::CONNECTION_ACK);
            ack.set_connectionid(player->id);

            session->send(ack);

            std::cout
                << "JOIN: "
                << player->playerName
                << std::endl;

            break;
        }

        case ClientMessage::PLAYER_INPUT:
        {
            auto playerIt = world_.players.find(playerId);
            if (playerIt == world_.players.end())
                return;
            auto& p = playerIt->second;
            p->input.CopyFrom(msg.input());
            break;
        }

        default:
            break;
        }
    }

    void GameServer::broadcast(const ServerMessage& msg)
    {
        for (auto& [id, session] : world_.sessions)
        {
            session->send(msg);
        }
    }

    void GameServer::updateTick()
    {
        tickTimer_.expires_after(std::chrono::milliseconds(1000 / TICK_RATE));
        tickTimer_.async_wait([this](auto ec)
            {
                if (ec) return;
                float dt = 1.f / TICK_RATE;

                MovementSystem::update(world_, dt);
                ShootingSystem::update(world_, dt);
                ProjectileSystem::update(world_, dt);
                CollisionSystem::update(world_);

                ServerMessage msg;
                msg.set_type(ServerMessage::GAME_STATE);

                auto state = msg.mutable_gamestate();

                for (auto& [id, p] : world_.players)
                    state->add_tanks()->CopyFrom(p->tankState);

                for (auto& proj : world_.projectiles)
                {
                    state->add_projectiles()->CopyFrom(proj);
                }

                broadcast(msg);

                updateTick();
            });
    }

    void GameServer::run()
    {
        io_.run();
    }
}