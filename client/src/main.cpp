#include <iostream>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"

int main(int ac, char **av)
{
    NetworkManager nm;
    UdpPacket tosend;
    TcpPacket tosend2;

    tosend.setID(1);
    tosend.setData(const_cast<char*>("coucou"));
    tosend.setSize(6);
    tosend.setQuery(2);

    tosend2.setData(const_cast<char*>("salut"));
    tosend2.setSize(5);
    tosend2.setQuery(3);
    nm.send(tosend);
    nm.send(tosend2);
    while (1)
    {
        IPacket *tmp = nm.getPacket();
        UdpPacket *udp = 0;
        TcpPacket *tcp = 0;
        if ((udp = dynamic_cast<UdpPacket*>(tmp)))
        {
            std::cout << "Udp Packet" << std::endl
                << "Id " << udp->getID()
                << " query " << udp->getQuery()
                << " size " << udp->getSize()
                << " data [";
            std::cout.flush();
            std::cout.write(reinterpret_cast<char*>(udp->getData()), udp->getSize());
            std::cout << "]" << std::endl;
            nm.send(tosend);
        }
        else if ((tcp = dynamic_cast<TcpPacket*>(tmp)))
        {
            std::cout << "Tcp Packet" << std::endl
                << "query " << tcp->getQuery()
                << " size " << tcp->getSize()
                << " data [";
            std::cout.flush();
            std::cout.write(reinterpret_cast<char*>(tcp->getData()), tcp->getSize());
            std::cout << "]" << std::endl;
            nm.send(tosend2);
        }
    }
    return 0;
}
