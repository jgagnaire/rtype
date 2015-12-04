#include "Network/UdpSocket.hh"

bool    UdpSocket::bind(unsigned short int port)
{
    _socket.setBlocking(false);
    return (_socket.bind(port) == sf::Socket::Done);
}

std::size_t     UdpSocket::receive(void *buf, std::size_t size,
        std::string &ip, unsigned short &port)
{
    sf::IpAddress   ipa;
    std::size_t     read;

    _socket.receive(buf, size, read, ipa, port);
    ip = ipa.toString();
    return (read);
}
