#pragma once

#include <vector>

struct Event
{
    int fd;

    bool readable;

    bool writable;
};

class IEventPoller
{
public:
    virtual ~IEventPoller() = default;

    virtual void addReadEvent(int fd) = 0;

    virtual void addWriteEvent(int fd) = 0;

    virtual void removeWriteEvent(int fd) = 0;

    virtual std::vector<Event> wait() = 0;
};