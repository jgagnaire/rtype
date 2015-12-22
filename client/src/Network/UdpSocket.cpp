#include "Network/UdpSocket.hh"

bool    UdpSocket::bind(unsigned short int port)
{
    _socket.setBlocking(false);
    return (_socket.bind(port) == sf::Socket::Done);
}

uint64_t     UdpSocket::receive(void *buf, uint64_t size,
        std::string &ip, unsigned short &port)
{
    sf::IpAddress   ipa;
    std::size_t     bytes_read;

    _socket.receive(buf, size, bytes_read, ipa, port);
    ip = ipa.toString();
    return (bytes_read);
}
