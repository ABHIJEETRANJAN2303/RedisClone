#include "buffer/Buffer.hpp"

void Buffer::append(const char *dataPtr,
                    size_t length)
{
    data.append(dataPtr, length);
}

void Buffer::append(const std::string &value)
{
    data.append(value);
}

const std::string &Buffer::getData() const
{
    return data;
}

size_t Buffer::size() const
{
    return data.size();
}

bool Buffer::empty() const
{
    return data.empty();
}

void Buffer::consume(size_t bytes)
{
    data.erase(0, bytes);
}

void Buffer::clear()
{
    data.clear();
}