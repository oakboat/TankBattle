#include "NetClient.h"

#include <iostream>

namespace TankBattle
{
    NetClient::NetClient()
        : socket_(io_)
    {
    }

    NetClient::~NetClient()
    {
        disconnect();
    }

    bool NetClient::connect(
        const std::string& host,
        uint16_t port)
    {
        try
        {
            boost::asio::ip::tcp::resolver resolver(io_);

            auto endpoints =
                resolver.resolve(
                    host,
                    std::to_string(port));

            boost::asio::connect(socket_, endpoints);

            connected_ = true;

            readHeader();

            networkThread_ =
                std::thread([this]()
                {
                    io_.run();
                });

            return true;
        }
        catch (std::exception& e)
        {
            std::cout
                << "Connect failed: "
                << e.what()
                << std::endl;

            return false;
        }
    }

    void NetClient::disconnect()
    {
        connected_ = false;

        boost::system::error_code ec;

        io_.stop();

        socket_.close(ec);

        if (networkThread_.joinable())
        {
            networkThread_.join();
        }
    }

    void NetClient::readHeader()
    {
        boost::asio::async_read(
            socket_,
            boost::asio::buffer(
                &header_,
                sizeof(PacketHeader)),
            [this](boost::system::error_code ec, std::size_t)
            {
                if (ec)
                {
                    disconnect();
                    return;
                }

                if (header_.size == 0 ||
                    header_.size > MAX_PACKET_SIZE)
                {
                    disconnect();
                    return;
                }

                readBody(header_.size);
            });
    }

    void NetClient::readBody(uint32_t size)
    {
        bodyBuffer_.resize(size);

        boost::asio::async_read(
            socket_,
            boost::asio::buffer(
                bodyBuffer_.data(),
                size),
            [this](boost::system::error_code ec, std::size_t)
            {
                if (ec)
                {
                    disconnect();
                    return;
                }

                ServerMessage msg;

                if (msg.ParseFromArray(
                        bodyBuffer_.data(),
                        (int)bodyBuffer_.size()))
                {
                    std::lock_guard lock(queueMutex_);

                    recvQueue_.push(msg);
                }

                readHeader();
            });
    }

    void NetClient::send(const ClientMessage& msg)
    {
        std::string body;

        if (!msg.SerializeToString(&body))
            return;

        PacketHeader hdr;

        hdr.size = (uint32_t)body.size();

        std::string packet;

        packet.resize(
            sizeof(PacketHeader) +
            body.size());

        memcpy(
            packet.data(),
            &hdr,
            sizeof(PacketHeader));

        memcpy(
            packet.data() + sizeof(PacketHeader),
            body.data(),
            body.size());

        bool idle;

        {
            std::lock_guard lock(sendMutex_);

            idle = sendQueue_.empty();

            sendQueue_.push(std::move(packet));
        }

        if (idle)
        {
            doWrite();
        }
    }

    void NetClient::doWrite()
    {
        std::lock_guard lock(sendMutex_);

        if (sendQueue_.empty())
            return;

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(sendQueue_.front()),
            [this](boost::system::error_code ec, std::size_t)
            {
                if (ec)
                {
                    disconnect();
                    return;
                }

                {
                    std::lock_guard lock(sendMutex_);

                    sendQueue_.pop();

                    if (sendQueue_.empty())
                        return;
                }

                doWrite();
            });
    }

    bool NetClient::pollMessage(ServerMessage& msg)
    {
        std::lock_guard lock(queueMutex_);

        if (recvQueue_.empty())
            return false;

        msg = recvQueue_.front();

        recvQueue_.pop();

        return true;
    }
}