#include "protocol/RESPParser.hpp"

#include <cstdlib>

std::optional<RESPCommand> RESPParser::parse(
    std::string &buffer)
{
    if (buffer.empty())
    {
        return std::nullopt;
    }

    if (buffer[0] != '*')
    {
        return std::nullopt;
    }

    size_t lineEnd = buffer.find("\r\n");

    if (lineEnd == std::string::npos)
    {
        return std::nullopt;
    }

    int argumentCount =
        std::stoi(buffer.substr(1, lineEnd - 1));

    size_t position = lineEnd + 2;

    RESPCommand command;

    for (int i = 0; i < argumentCount; ++i)
    {
        if (position >= buffer.size())
        {
            return std::nullopt;
        }

        if (buffer[position] != '$')
        {
            return std::nullopt;
        }

        size_t lengthEnd =
            buffer.find("\r\n", position);

        if (lengthEnd == std::string::npos)
        {
            return std::nullopt;
        }

        int stringLength =
            std::stoi(
                buffer.substr(
                    position + 1,
                    lengthEnd - position - 1));

        position = lengthEnd + 2;

        if (position + stringLength + 2 >
            buffer.size())
        {
            return std::nullopt;
        }

        std::string value =
            buffer.substr(position, stringLength);

        command.arguments.push_back(value);

        position += stringLength + 2;
    }

    buffer.erase(0, position);

    return command;
}