#pragma once

#include "poller/KqueuePoller.hpp"

class TcpServer;

class EventLoop
{
public:
    void run(KqueuePoller &poller,
             TcpServer &server);
};