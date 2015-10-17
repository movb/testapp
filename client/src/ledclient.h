#ifndef LEDCLIENT_H
#define LEDCLIENT_H

#include <string>
#include <stdexcept>

#include <boost/asio.hpp>

struct LedClientException : public std::runtime_error {
    LedClientException(const std::string& message):
        runtime_error(message) {}
};

class LedClient
{
public:
    LedClient();
    LedClient(std::string ip, unsigned int port);

    void connect(std::string ip, unsigned int port);
    std::string sendCommand(const std::string& command);

    size_t readComplete(const boost::system::error_code & err, size_t bytes);
private:
    boost::asio::io_service m_service;
    boost::asio::ip::tcp::socket m_socket;
    constexpr static unsigned int bufferLength { 1024 };
    int m_already_read { 0 };
    char m_buffer[bufferLength];
    bool m_started { false };
};

#endif // LEDCLIENT_H
