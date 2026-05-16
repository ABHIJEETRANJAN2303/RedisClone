#include "server/Connection.hpp"

Connection::Connection(int fd)
    : clientFd(fd)
{
}

int Connection::getFd() const
{
    return clientFd;
}

Buffer &Connection::getReadBuffer()
{
    return readBuffer;
}

Buffer &Connection::getWriteBuffer()
{
    return writeBuffer;
}

RESPParser &Connection::getParser()
{
    return parser;
}