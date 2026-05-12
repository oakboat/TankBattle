#pragma once

#include <cstdint>

namespace TankBattle
{
#pragma pack(push, 1)

    struct PacketHeader
    {
        uint32_t size;
    };

#pragma pack(pop)

    constexpr uint32_t MAX_PACKET_SIZE = 1024 * 64;
}