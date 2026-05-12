#include "GameClient.h"
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[])
{
    std::string serverHost = "127.0.0.1";
    uint16_t port = 12345;
    std::string playerName = "Player" + std::to_string(rand() % 1000);

    if (argc > 1)
        serverHost = argv[1];

    if (argc > 2)
        port = static_cast<uint16_t>(std::stoi(argv[2]));

    if (argc > 3)
        playerName = argv[3];

    TankBattle::GameClient client;

    if (!client.init(serverHost, port, playerName))
    {
        std::cerr << "[Client] init failed\n";
        return 1;
    }

    client.run();

    return 0;
}