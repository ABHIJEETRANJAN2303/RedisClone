#include "server/SocketUtils.hpp"

#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

void SocketUtils::setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags < 0)
    {
        throw std::runtime_error("fcntl(F_GETFL) failed");
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        throw std::runtime_error("fcntl(F_SETFL) failed");
    }
}