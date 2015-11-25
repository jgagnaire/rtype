#include <iostream>
#include <limits.h>
#include <cstring>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"

NetworkManager::NetworkManager(const std::string &ip, unsigned short port):
    _udp(*new UdpSocket()), _tcp(*new TcpSocket()),
    _tcpIp(ip), _tcpPort(port)
{
    _tcp.connect(ip, port);
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
        totalSize += sizeof(TcpHeader);
    else if ((udpPacket = dynamic_cast<const UdpPacket*>(tmp)))
        totalSize += sizeof(UdpHeader);
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

void        NetworkManager::receiveUdp()
{
    UdpPacket *packet = new UdpPacket();
    std::string ip;
    unsigned short port;
    char *buf = new char[std::numeric_limits<unsigned short>::max()];
    char *tmp;

    std::size_t readed = _udp.receive(buf,
            std::numeric_limits<unsigned short>::max(),
            ip, port);
    if (readed)
    {
        static_cast<UdpHeader&>(packet->getHeader()) = *reinterpret_cast<UdpHeader*>(buf);
        if (packet->getSize() > 0)
        {
            tmp = new char[packet->getSize()];
            std::memcpy(tmp, buf + sizeof(UdpHeader), packet->getSize());
            packet->setData(tmp);
        }
        else
            packet->setData(0);
        _packets.push_back(packet);
    }
    else
        delete packet;
    delete[] buf;
}

void        NetworkManager::receiveTcp()
{
    TcpPacket *packet = new TcpPacket();
    char *buf = 0;

    std::size_t readed = _tcp.receive(&(packet->getHeader()),
            sizeof(TcpHeader));
    if (readed)
    {
        if (packet->getSize())
        {
            buf = new char[packet->getSize()];
            readed = _tcp.receive(buf, packet->getSize());
        }
        packet->setData(buf);
        _packets.push_back(packet);
    }
    else
        delete packet;
}

IPacket     *NetworkManager::getPacket()
{
    IPacket *tmp = 0;


    if (_packets.empty())
    {
        this->receiveUdp();
        this->receiveTcp();
    }
    if (_packets.empty() == false)
    {
        tmp = _packets.front();
        _packets.pop_front();
        return tmp;
    }
    return 0;
}
