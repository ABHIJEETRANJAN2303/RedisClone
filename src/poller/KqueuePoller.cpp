#include "poller/KqueuePoller.hpp"

#include <stdexcept>
#include <sys/event.h>
#include <unistd.h>

KqueuePoller::KqueuePoller()
    : kqueueFd(kqueue()),
      events(MAX_EVENTS)
{
    if (kqueueFd < 0)
    {
        throw std::runtime_error("Failed to create kqueue");
    }
}

KqueuePoller::~KqueuePoller()
{
    close(kqueueFd);
}

void KqueuePoller::addReadEvent(int fd)
{
    struct kevent changeEvent;

    EV_SET(&changeEvent,
           fd,
           EVFILT_READ,
           EV_ADD,
           0,
           0,
           nullptr);

    if (kevent(kqueueFd,
               &changeEvent,
               1,
               nullptr,
               0,
               nullptr) < 0)
    {
        throw std::runtime_error("kevent read register failed");
    }
}

void KqueuePoller::addWriteEvent(int fd)
{
    struct kevent changeEvent;

    EV_SET(&changeEvent,
           fd,
           EVFILT_WRITE,
           EV_ADD,
           0,
           0,
           nullptr);

    if (kevent(kqueueFd,
               &changeEvent,
               1,
               nullptr,
               0,
               nullptr) < 0)
    {
        throw std::runtime_error("kevent write register failed");
    }
}

void KqueuePoller::removeWriteEvent(int fd)
{
    struct kevent changeEvent;

    EV_SET(&changeEvent,
           fd,
           EVFILT_WRITE,
           EV_DELETE,
           0,
           0,
           nullptr);

    kevent(kqueueFd,
           &changeEvent,
           1,
           nullptr,
           0,
           nullptr);
}

std::vector<Event> KqueuePoller::wait()
{
    int eventCount = kevent(kqueueFd,
                            nullptr,
                            0,
                            events.data(),
                            MAX_EVENTS,
                            nullptr);

    if (eventCount < 0)
    {
        throw std::runtime_error("kevent wait failed");
    }

    std::vector<Event> readyEvents;

    for (int i = 0; i < eventCount; ++i)
    {
        Event event{};

        event.fd =
            static_cast<int>(events[i].ident);

        event.readable =
            events[i].filter == EVFILT_READ;

        event.writable =
            events[i].filter == EVFILT_WRITE;

        readyEvents.push_back(event);
    }

    return readyEvents;
}