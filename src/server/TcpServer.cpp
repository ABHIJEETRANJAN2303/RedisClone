#include "server/TcpServer.hpp"
#include "common/Logger.hpp"
#include "server/SocketUtils.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

TcpServer::TcpServer(int port)
    : serverFd(-1),
      port(port),
      commandDispatcher(keyValueStore)
{
}

void TcpServer::setupSocket()
{
    serverFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverFd < 0)
    {
        Logger::error("Failed to create socket");

        std::exit(EXIT_FAILURE);
    }

    SocketUtils::setNonBlocking(serverFd);

    int opt = 1;

    setsockopt(serverFd,
               SOL_SOCKET,
               SO_REUSEADDR,
               &opt,
               sizeof(opt));

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverFd,
             reinterpret_cast<sockaddr *>(
                 &serverAddress),
             sizeof(serverAddress)) < 0)
    {

        Logger::error("Bind failed");

        close(serverFd);

        std::exit(EXIT_FAILURE);
    }

    if (listen(serverFd, SOMAXCONN) < 0)
    {

        Logger::error("Listen failed");

        close(serverFd);

        std::exit(EXIT_FAILURE);
    }

    poller.addReadEvent(serverFd);

    Logger::info(
        "Server listening on port " +
        std::to_string(port));
}

void TcpServer::acceptConnections()
{
    while (true)
    {

        sockaddr_in clientAddress{};

        socklen_t clientLength =
            sizeof(clientAddress);

        int clientFd = accept(
            serverFd,
            reinterpret_cast<sockaddr *>(
                &clientAddress),
            &clientLength);

        if (clientFd < 0)
        {

            if (errno == EAGAIN ||
                errno == EWOULDBLOCK)
            {
                break;
            }

            Logger::error("Accept failed");

            break;
        }

        SocketUtils::setNonBlocking(
            clientFd);

        poller.addReadEvent(clientFd);

        connections.emplace(
            clientFd,
            Connection(clientFd));

        Logger::info(
            "Accepted new client fd " +
            std::to_string(clientFd));
    }
}

void TcpServer::handleClientRead(
    int clientFd)
{
    char tempBuffer[1024];

    while (true)
    {

        ssize_t bytesRead = recv(
            clientFd,
            tempBuffer,
            sizeof(tempBuffer),
            0);

        if (bytesRead > 0)
        {

            Connection &connection =
                connections.at(clientFd);

            Buffer &readBuffer =
                connection.getReadBuffer();

            readBuffer.append(
                tempBuffer,
                bytesRead);

            while (true)
            {

                std::optional<RESPCommand>
                    command =
                        connection
                            .getParser()
                            .parse(
                                const_cast<
                                    std::string &>(
                                    readBuffer
                                        .getData()));

                if (!command.has_value())
                {
                    break;
                }

                Logger::info(
                    "Parsed RESP command fd " +
                    std::to_string(
                        clientFd));

                std::string response =
                    commandDispatcher
                        .dispatch(
                            command.value());

                connection
                    .getWriteBuffer()
                    .append(response);

                poller.addWriteEvent(
                    clientFd);
            }
        }
        else if (bytesRead == 0)
        {

            Logger::info(
                "Client disconnected fd " +
                std::to_string(
                    clientFd));

            close(clientFd);

            connections.erase(clientFd);

            break;
        }
        else
        {

            if (errno == EAGAIN ||
                errno ==
                    EWOULDBLOCK)
            {
                break;
            }

            Logger::error(
                "recv failed fd " +
                std::to_string(
                    clientFd));

            close(clientFd);

            connections.erase(clientFd);

            break;
        }
    }
}

void TcpServer::handleClientWrite(
    int clientFd)
{
    Connection &connection =
        connections.at(clientFd);

    Buffer &writeBuffer =
        connection.getWriteBuffer();

    if (writeBuffer.empty())
    {

        poller.removeWriteEvent(
            clientFd);

        return;
    }

    ssize_t bytesSent = send(
        clientFd,
        writeBuffer
            .getData()
            .c_str(),
        writeBuffer.size(),
        0);

    if (bytesSent > 0)
    {

        writeBuffer.consume(
            bytesSent);

        if (writeBuffer.empty())
        {

            poller.removeWriteEvent(
                clientFd);
        }
    }
    else if (bytesSent < 0)
    {

        if (errno == EAGAIN ||
            errno ==
                EWOULDBLOCK)
        {
            return;
        }

        Logger::error(
            "send failed fd " +
            std::to_string(
                clientFd));

        close(clientFd);

        connections.erase(clientFd);
    }
}

void TcpServer::handleEvent(
    int fd,
    bool readable,
    bool writable)
{
    if (fd == serverFd && // Listening Socket Case
        readable)
    {

        acceptConnections();

        return;
    }

    if (readable)
    {
        handleClientRead(fd);
    }

    if (writable)
    {
        handleClientWrite(fd);
    }
}

void TcpServer::start()
{
    setupSocket();

    eventLoop.run(
        poller,
        *this);
}