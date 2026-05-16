#pragma once

#include <string>

class Buffer
{
public:
    void append(const char *data,
                size_t length);

    void append(const std::string &value);

    const std::string &getData() const;

    size_t size() const;

    bool empty() const;

    void consume(size_t bytes);

    void clear();

private:
    std::string data;
};