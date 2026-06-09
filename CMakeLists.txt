cmake_minimum_required(VERSION 3.16)

project(RedisClone)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(redis_clone
    src/main.cpp

    src/server/TcpServer.cpp
    src/server/EventLoop.cpp
    src/server/Connection.cpp
    src/server/SocketUtils.cpp

    src/poller/KqueuePoller.cpp

    src/common/Logger.cpp

    src/buffer/Buffer.cpp

    src/protocol/RESPParser.cpp
    src/protocol/RESPEncoder.cpp

    src/commands/CommandDispatcher.cpp
    src/commands/StringCommands.cpp
    src/commands/ServerCommands.cpp

    src/storage/KeyValueStore.cpp
    src/storage/ExpirationManager.cpp
)

target_include_directories(redis_clone PRIVATE src)
