#pragma once

#include "protocol/RESPTypes.hpp"

#include <optional>
#include <string>

class RESPParser
{
public:
    std::optional<RESPCommand> parse(
        std::string &buffer);
};