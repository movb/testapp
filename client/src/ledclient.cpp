#include "ledclient.h"

#include <boost/bind.hpp>

using namespace boost::asio;

LedClient::LedClient()
    :
      m_socket(m_service)
{
}

LedClient::LedClient(std::string ip, unsigned int port)
    :
      m_socket(m_service)
{
    connect(ip,port);
}

void LedClient::connect(std::string ip, unsigned int port)
{
    try {
        ip::tcp::endpoint ep( ip::address::from_string(ip), port);
        m_socket.connect(ep);
    } catch (boost::system::system_error & err) {
        throw LedClientException(err.what());
    }
}

std::string LedClient::sendCommand(const std::string &command)
{
    try {
        m_socket.write_some(buffer(command + "\n"));
        m_already_read = 0;
        read(m_socket, buffer(m_buffer),
             boost::bind(&LedClient::readComplete, this, _1, _2));
        return std::string(m_buffer, m_already_read);
    } catch (boost::system::system_error & err) {
        throw LedClientException(err.what());
    }
}

size_t LedClient::readComplete(const boost::system::error_code &err, size_t bytes) {
    if ( err) return 0;
    m_already_read = bytes;
    bool found = std::find(m_buffer, m_buffer + bytes, '\n') < m_buffer + bytes;
    return found ? 0 : 1;
}

