#pragma once

#include "commands/CommandDispatcher.hpp"
#include "poller/KqueuePoller.hpp"
#include "server/Connection.hpp"
#include "server/EventLoop.hpp"
#include "storage/KeyValueStore.hpp"

#include <unordered_map>

class TcpServer
{
public:
    TcpServer(int port);

    void start();

    void handleEvent(int fd,
                     bool readable,
                     bool writable);

private:
    int serverFd;
    int port;

    KqueuePoller poller;

    EventLoop eventLoop;

    std::unordered_map<int, Connection> connections;

    KeyValueStore keyValueStore;

    CommandDispatcher commandDispatcher;

    void setupSocket();

    void acceptConnections();

    void handleClientRead(int clientFd);

    void handleClientWrite(int clientFd);
};