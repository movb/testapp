#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <string>
#include <stdint.h>

class TcpServer
{
public:
    TcpServer(std::string address, uint16_t port);
};

#endif // TCPSERVER_H
