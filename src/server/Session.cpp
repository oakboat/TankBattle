#include "Session.h"
#include "GameServer.h"
#include <iostream>

namespace TankBattle
{
    Session::Session(boost::asio::io_context& io, GameServer* server, uint32_t id)
        : socket(io),
          server_(server),
          id(id)
    {
    }

    void Session::start()
    {
        readHeader();
    }

    void Session::readHeader()
    {
        auto self(shared_from_this());

        boost::asio::async_read(
            socket,
            boost::asio::buffer(&header_, sizeof(PacketHeader)),
            [this, self](boost::system::error_code ec, std::size_t)
            {
                if (ec)
                {
                    server_->disconnect(id);
                    return;
                }

                if (header_.size == 0 || header_.size > MAX_PACKET_SIZE)
                {
                    server_->disconnect(id);
                    return;
                }

                readBody(header_.size);
            });
    }

    void Session::readBody(uint32_t size)
    {
        auto self(shared_from_this());

        bodyBuffer_.resize(size);

        boost::asio::async_read(
            socket,
            boost::asio::buffer(bodyBuffer_.data(), size),
            [this, self](boost::system::error_code ec, std::size_t)
            {
                if (ec)
                {
                    server_->disconnect(id);
                    return;
                }

                ClientMessage msg;

                if (msg.ParseFromArray(bodyBuffer_.data(), (int)bodyBuffer_.size()))
                {
                    handleMessage(msg);
                }

                readHeader();
            });
    }

    void Session::handleMessage(const ClientMessage& msg)
    {
        server_->handleClientMessage(id, msg);
    }

    void Session::send(const ServerMessage& msg)
    {
        std::string body;

        if (!msg.SerializeToString(&body))
            return;

        PacketHeader hdr;
        hdr.size = (uint32_t)body.size();

        std::string packet;
        packet.resize(sizeof(PacketHeader) + body.size());

        memcpy(packet.data(), &hdr, sizeof(PacketHeader));
        memcpy(packet.data() + sizeof(PacketHeader), body.data(), body.size());

        bool idle = sendQueue_.empty();

        sendQueue_.push(std::move(packet));

        if (idle)
        {
            doWrite();
        }
    }

    void Session::doWrite()
    {
        if (sendQueue_.empty())
            return;

        auto self(shared_from_this());

        boost::asio::async_write(
            socket,
            boost::asio::buffer(sendQueue_.front()),
            [this, self](boost::system::error_code ec, std::size_t)
            {
                if (ec)
                {
                    server_->disconnect(id);
                    return;
                }

                sendQueue_.pop();

                if (!sendQueue_.empty())
                {
                    doWrite();
                }
            });
    }
}