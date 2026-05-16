#pragma once

#include "protocol/RESPTypes.hpp"
#include "storage/KeyValueStore.hpp"

#include <string>

class CommandDispatcher
{
public:
    explicit CommandDispatcher(
        KeyValueStore &keyValueStore);

    std::string dispatch(
        const RESPCommand &command);

private:
    KeyValueStore &keyValueStore;

    std::string handlePing(
        const RESPCommand &command);

    std::string handleSet(
        const RESPCommand &command);

    std::string handleGet(
        const RESPCommand &command);
};