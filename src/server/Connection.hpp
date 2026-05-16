#pragma once

#include "buffer/Buffer.hpp"
#include "protocol/RESPParser.hpp"

class Connection
{
public:
    explicit Connection(int fd);

    int getFd() const;

    Buffer &getReadBuffer();

    Buffer &getWriteBuffer();

    RESPParser &getParser();

private:
    int clientFd;

    Buffer readBuffer;

    Buffer writeBuffer;

    RESPParser parser;
};