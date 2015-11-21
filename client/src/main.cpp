#include <iostream>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"

int main(int ac, char **av)
{
    NetworkManager nm;
    UdpPacket tosend;
    TcpPacket tosend2;

    if (ac == 1)
    {
        std::cout << "choose 1: Udp, 2: Tcp" << std::endl;
        return (0);
    }
    if (av[1][0] == '1')
    {
        std::cout << "Sending a udp packet like this {6, 2, 1, \"coucou\"}"
            << " 127.0.0.1 4445" << std::endl;
        tosend.setID(1);
        tosend.setData(const_cast<char*>("coucou"));
        tosend.setSize(6);
        tosend.setQuery(2);
        nm.send(tosend);
    }
    else if (av[1][0] == '2')
    {
        std::cout << "Connect and Sending a tcp packet like this {6, 2, \"coucou\"}"
            << " 127.0.0.1 4443" << std::endl;
        tosend2.setData(const_cast<char*>("coucou"));
        tosend2.setSize(6);
        tosend2.setQuery(2);
        nm.send(tosend2);
    }
    return 0;
}
