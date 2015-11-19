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
