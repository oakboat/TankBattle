#pragma once

#include <boost/asio.hpp>
#include <unordered_map>
#include <memory>
#include <vector>

#include "../common/Constants.h"
#include "world/World.h"
#include "game.pb.h"

namespace TankBattle
{
    class Session;

    class GameServer
    {
    public:
        GameServer(uint16_t port);

        void run();

    private:
        void startAccept();
        void disconnect(uint32_t id);
        void handleClientMessage(uint32_t playerId, const ClientMessage& msg);

        void updateTick();
        void broadcast(const ServerMessage& msg);

    private:
        boost::asio::io_context io_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::steady_timer tickTimer_;

        World world_;

        friend class Session;
    };
}