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

    if ((tcpPacket = dynamic_cast<const TcpPacket*>(tmp)))
    {
        std::size_t totalSize = sizeof(tcpPacket->getHeader())
                                + tcpPacket->getSize();
        char *buf = new char[totalSize];
        std::memcpy(buf, &(tcpPacket->getHeader()),
                totalSize - tcpPacket->getSize());
        std::memcpy(buf + sizeof(tcpPacket->getHeader()),
                tcpPacket->getData(),
                tcpPacket->getSize());
        _tcp.send(buf, totalSize);
    }
    else if ((udpPacket = dynamic_cast<const UdpPacket*>(tmp)))
    {
        std::size_t totalSize = sizeof(udpPacket->getHeader())
                                + udpPacket->getSize();
        char *buf = new char[totalSize];
        std::memcpy(buf, &(udpPacket->getHeader()),
                sizeof(udpPacket->getHeader()));
        std::memcpy(buf + sizeof(udpPacket->getHeader()),
                udpPacket->getData(),
                udpPacket->getSize());
        _udp.send(buf, totalSize, "127.0.0.1", 4445);
    }
}
