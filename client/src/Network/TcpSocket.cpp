#include "Network/TcpSocket.hh"

bool            TcpSocket::connect(const std::string &ip,
        unsigned short int port)
{
    _socket.setBlocking(false);
    return (_socket.connect(ip, port) == sf::Socket::Done);
}

std::size_t     TcpSocket::send(const void *buf, std::size_t size)
{
    std::size_t sent;
    _socket.send(buf, size, sent);
    return (sent);
}

std::size_t     TcpSocket::receive(void *buf, std::size_t size)
{
    std::size_t read;
    _socket.receive(buf, size, read);
    return (read);
}
