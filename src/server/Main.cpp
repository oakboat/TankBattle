#include "GameServer.h"

int main()
{
    TankBattle::GameServer server(12345);

    server.run();

    return 0;
}