#pragma once

#include "poller/IEventPoller.hpp"

#include <vector>

struct kevent;

class KqueuePoller : public IEventPoller
{
public:
    KqueuePoller();

    ~KqueuePoller();

    void addReadEvent(int fd) override;

    void addWriteEvent(int fd) override;

    void removeWriteEvent(int fd) override;

    std::vector<Event> wait() override;

private:
    int kqueueFd;

    static constexpr int MAX_EVENTS = 32;

    std::vector<struct kevent> events;
};