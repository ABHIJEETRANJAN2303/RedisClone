#pragma once

#include "storage/ValueObject.hpp"

#include <optional>
#include <string>
#include <unordered_map>

class KeyValueStore
{
public:
    void set(const std::string &key,
             const std::string &value);

    void setWithTTL(
        const std::string &key,
        const std::string &value,
        int ttlSeconds);

    std::optional<std::string> get(
        const std::string &key);

private:
    std::unordered_map<
        std::string,
        ValueObject>
        store;

    bool isExpired(
        const ValueObject &valueObject) const;
};