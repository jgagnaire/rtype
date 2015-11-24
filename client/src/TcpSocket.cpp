#include "TcpSocket.hh"

TcpSocket::TcpSocket()
{}

TcpSocket::~TcpSocket()
{}

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

Header   &TcpPacket::getHeader()
{
     return _header;
}

const Header   &TcpPacket::getHeader() const
{
     return _header;
}

uint16_t    TcpPacket::getSize() const
{
    return _header.size;
}

uint16_t    TcpPacket::getQuery() const
{
    return _header.query;
}

void        *TcpPacket::getData() const
{
    return _data;
}

void        TcpPacket::setSize(uint16_t size)
{
    _header.size = size;
}

void        TcpPacket::setQuery(uint16_t query)
{
    _header.query = query;
}

void        TcpPacket::setData(void *data)
{
    _data = data;
}
