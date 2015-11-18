#include "UdpSocket.hh"

UdpSocket::UdpSocket()
{}

UdpSocket::~UdpSocket()
{}

bool    UdpSocket::bind(unsigned short int port)
{
    return (_socket.bind(port) == sf::Socket::Done);
}

void    UdpSocket::unbind()
{
    _socket.unbind();
}

std::size_t     UdpSocket::send(const void *buf, std::size_t size,
        const std::string &ip, unsigned short int port)
{
    return (_socket.send(buf, size, ip, port) == sf::Socket::Done);
}

std::size_t     UdpSocket::receive(void *buf, std::size_t size,
        std::string &ip, unsigned short &port)
{
    sf::IpAddress   ipa;
    std::size_t     readed;

    _socket.receive(buf, size, readed, ipa, port);
    ip = ipa.toString();
    return (readed);
}
