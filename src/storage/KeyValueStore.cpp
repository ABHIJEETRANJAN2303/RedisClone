#include "storage/KeyValueStore.hpp"

void KeyValueStore::set(
    const std::string &key,
    const std::string &value)
{
    ValueObject object;

    object.value = value;

    object.expirationTime = std::nullopt;

    store[key] = object;
}

void KeyValueStore::setWithTTL(
    const std::string &key,
    const std::string &value,
    int ttlSeconds)
{
    ValueObject object;

    object.value = value;

    object.expirationTime =
        std::chrono::steady_clock::now() +
        std::chrono::seconds(ttlSeconds);

    store[key] = object;
}

bool KeyValueStore::isExpired(
    const ValueObject &valueObject) const
{
    if (!valueObject
             .expirationTime
             .has_value())
    {
        return false;
    }

    return std::chrono::steady_clock::now() >=
           valueObject
               .expirationTime
               .value();
}

std::optional<std::string>
KeyValueStore::get(
    const std::string &key)
{
    auto iterator = store.find(key);

    if (iterator == store.end())
    {
        return std::nullopt;
    }

    if (isExpired(iterator->second))
    {

        store.erase(iterator);

        return std::nullopt;
    }

    return iterator->second.value;
}