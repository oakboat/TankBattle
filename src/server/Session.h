#pragma once

#include <boost/asio.hpp>
#include <queue>
#include <memory>
#include "game.pb.h"
#include "../common/Packet.h"

namespace TankBattle
{
    class GameServer;

    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(boost::asio::io_context& io, GameServer* server, uint32_t id);

        void start();

        void send(const ServerMessage& msg);

        boost::asio::ip::tcp::socket socket;

        uint32_t id;

        float lastShootTime = 0.f;

    private:
        void readHeader();

        void readBody(uint32_t size);

        void handleMessage(const ClientMessage& msg);

        void doWrite();

    private:
        GameServer* server_;

        PacketHeader header_;

        std::vector<char> bodyBuffer_;

        std::queue<std::string> sendQueue_;

        bool writing_ = false;
    };
}