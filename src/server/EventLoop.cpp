#include "server/EventLoop.hpp"
#include "server/TcpServer.hpp"

void EventLoop::run(KqueuePoller &poller,
                    TcpServer &server)
{
    while (true)
    {
        std::vector<Event> events =
            poller.wait();

        for (const Event &event : events)
        {
            server.handleEvent(
                event.fd,
                event.readable,
                event.writable);
        }
    }
}