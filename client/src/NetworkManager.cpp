#include <cstring>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"

NetworkManager::NetworkManager():
    _udp(*new UdpSocket()), _tcp(*new TcpSocket())
{
    _tcp.connect("127.0.0.1", 4443);
    _udp.bind(4444);
}

NetworkManager::~NetworkManager()
{
    delete &_udp;
    delete &_tcp;
}

void    NetworkManager::send(const IPacket &packet)
{
    const IPacket *tmp = &packet;
    const TcpPacket *tcpPacket = 0;
    const UdpPacket *udpPacket = 0;
    std::size_t totalSize = tmp->getSize();
    char *buf = 0;

    if ((tcpPacket = dynamic_cast<const TcpPacket*>(tmp)))
        totalSize += sizeof(tcpPacket->getHeader());
    else if ((udpPacket = dynamic_cast<const UdpPacket*>(tmp)))
        totalSize += sizeof(udpPacket->getHeader());
    buf = new char[totalSize];
    std::memcpy(buf, &(tmp->getHeader()),
            totalSize - tmp->getSize());
    std::memcpy(buf + totalSize - tmp->getSize(),
            tmp->getData(), tmp->getSize());
    if (tcpPacket)
        _tcp.send(buf, totalSize);
    else if (udpPacket)
        _udp.send(buf, totalSize, "127.0.0.1", 4445);
}
