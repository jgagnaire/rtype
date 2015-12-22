#include "Network/TcpSocket.hh"

bool            TcpSocket::connect(const std::string &ip,
        unsigned short int port)
{
    _socket.setBlocking(false);
    return (_socket.connect(ip, port) == sf::Socket::Done);
}

uint64_t     TcpSocket::send(const void *buf, uint64_t size)
{
	std::size_t sent;
    _socket.send(buf, size, sent);
    return (sent);
}

uint64_t     TcpSocket::receive(void *buf, uint64_t size)
{
    std::size_t read;
    _socket.receive(buf, size, read);
    return (read);
}
