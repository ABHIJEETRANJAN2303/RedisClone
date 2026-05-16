#pragma once

#include <chrono>
#include <optional>
#include <string>

struct ValueObject
{
    std::string value;

    std::optional<
        std::chrono::steady_clock::time_point>
        expirationTime;
};