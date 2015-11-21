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

Header   &UdpPacket::getHeader()
{
     return _header;
}

const Header   &UdpPacket::getHeader() const
{
     return _header;
}

uint16_t    UdpPacket::getSize() const
{
    return _header.size;
}

uint16_t    UdpPacket::getQuery() const
{
    return _header.query;
}

uint64_t    UdpPacket::getID() const
{
    return _header.id;
}

void        *UdpPacket::getData() const
{
    return _data;
}

void        UdpPacket::setSize(uint16_t size)
{
    _header.size = size;
}

void        UdpPacket::setQuery(uint16_t query)
{
    _header.query = query;
}

void        UdpPacket::setID(uint64_t id)
{
     _header.id = id;
}

void        UdpPacket::setData(void *data)
{
    _data = data;
}
