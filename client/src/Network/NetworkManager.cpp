#include <iostream>
#include <limits.h>
#include <cstring>
#include "Network/NetworkManager.hh"
#include "Network/UdpSocket.hh"
#include "Network/TcpSocket.hh"

NetworkManager::NetworkManager(const std::string &ip,
        const std::string &udpIp):
    _udp(*new UdpSocket()), _tcp(*new TcpSocket()),
    _tcpIp(ip), _udpIp(udpIp)
{
    _tcp.connect(ip, 1119);
    _udp.bind(1726);
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
        _udp.send(buf, totalSize, _udpIp, 1725);
}

void        NetworkManager::receiveUdp()
{
    UdpPacket *packet = new UdpPacket();
    std::string ip;
    unsigned short port;
    char *buf = new char[std::numeric_limits<unsigned short>::max()];
    char *tmp;

    std::size_t read = _udp.receive(buf,
            std::numeric_limits<unsigned short>::max(),
            ip, port);
    if (read)
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

    std::size_t read = _tcp.receive(&(packet->getHeader()),
            sizeof(TcpHeader));
    if (read)
    {
        if (packet->getSize())
        {
            buf = new char[packet->getSize()];
            read = _tcp.receive(buf, packet->getSize());
        }
        packet->setData(buf);
        std::cout << "Tcp {" << packet->getSize() <<
            ", " << packet->getQuery() << "}" << std::endl;
        if (static_cast<Codes>(packet->getQuery()) == Codes::Ping)
        {
            TcpPacket   p;
            p.setSize(0);
            p.setData(0);
            p.setQuery(static_cast<uint16_t>(Codes::Pong));
            this->send(p);
            delete packet;
            packet = 0;
        }
        else
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
