#include "TcpSocket.hh"

TcpSocket::TcpSocket()
{}

TcpSocket::~TcpSocket()
{}

bool            TcpSocket::connect(const std::string &ip,
        unsigned short int port)
{
    return (_socket.connect(ip, port) == sf::Socket::Done);
}

std::size_t     TcpSocket::send(const void *buf, std::size_t size)
{
    std::size_t sended;
    _socket.send(buf, size, sended);
    return (sended);
}

std::size_t     TcpSocket::receive(void *buf, std::size_t size)
{
    std::size_t readed;
    _socket.receive(buf, size, readed);
    return (readed);
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
