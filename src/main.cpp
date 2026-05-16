#include "server/TcpServer.hpp"

int main()
{
    TcpServer server(6379);

    server.start();

    return 0;
}