#include "commands/CommandDispatcher.hpp"
#include "protocol/RESPEncoder.hpp"

CommandDispatcher::CommandDispatcher(
    KeyValueStore &keyValueStore)
    : keyValueStore(keyValueStore)
{
}

std::string CommandDispatcher::dispatch(
    const RESPCommand &command)
{
    if (command.arguments.empty())
    {
        return RESPEncoder::encodeError(
            "empty command");
    }

    const std::string &commandName =
        command.arguments[0];

    if (commandName == "PING")
    {
        return handlePing(command);
    }

    if (commandName == "SET")
    {
        return handleSet(command);
    }

    if (commandName == "GET")
    {
        return handleGet(command);
    }

    return RESPEncoder::encodeError(
        "unknown command");
}

std::string CommandDispatcher::handlePing(
    const RESPCommand &command)
{
    return RESPEncoder::encodeSimpleString(
        "PONG");
}

std::string CommandDispatcher::handleSet(
    const RESPCommand &command)
{
    if (command.arguments.size() != 3 &&
        command.arguments.size() != 5)
    {

        return RESPEncoder::encodeError(
            "wrong number of arguments for SET");
    }

    const std::string &key =
        command.arguments[1];

    const std::string &value =
        command.arguments[2];

    if (command.arguments.size() == 3)
    {

        keyValueStore.set(
            key,
            value);

        return RESPEncoder::encodeSimpleString(
            "OK");
    }

    const std::string &option =
        command.arguments[3];

    const std::string &ttlValue =
        command.arguments[4];

    if (option != "EX")
    {

        return RESPEncoder::encodeError(
            "unsupported SET option");
    }

    int ttlSeconds =
        std::stoi(ttlValue);

    keyValueStore.setWithTTL(
        key,
        value,
        ttlSeconds);

    return RESPEncoder::encodeSimpleString(
        "OK");
}

std::string CommandDispatcher::handleGet(
    const RESPCommand &command)
{
    if (command.arguments.size() != 2)
    {

        return RESPEncoder::encodeError(
            "wrong number of arguments for GET");
    }

    const std::string &key =
        command.arguments[1];

    std::optional<std::string> value =
        keyValueStore.get(key);

    if (!value.has_value())
    {

        return RESPEncoder ::encodeNullBulkString();
    }

    return RESPEncoder ::encodeBulkString(
        value.value());
}