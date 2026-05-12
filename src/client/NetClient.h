#pragma once

#include <boost/asio.hpp>
#include <queue>
#include <mutex>
#include <thread>

#include "../common/Packet.h"
#include "game.pb.h"

namespace TankBattle
{
    class NetClient
    {
    public:
        NetClient();

        ~NetClient();

        bool connect(
            const std::string& host,
            uint16_t port);

        void disconnect();

        void send(const ClientMessage& msg);

        bool pollMessage(ServerMessage& msg);

    private:
        void readHeader();

        void readBody(uint32_t size);

        void doWrite();

    private:
        boost::asio::io_context io_;

        boost::asio::ip::tcp::socket socket_;

        std::thread networkThread_;

        PacketHeader header_;

        std::vector<char> bodyBuffer_;

        std::mutex queueMutex_;

        std::queue<ServerMessage> recvQueue_;

        std::mutex sendMutex_;

        std::queue<std::string> sendQueue_;

        bool connected_ = false;
    };
}