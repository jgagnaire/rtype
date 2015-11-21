#include <iostream>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"

int main()
{
    NetworkManager nm;
    UdpPacket tosend;

    tosend.setID(1);
    tosend.setData(const_cast<char*>("coucou"));
    tosend.setSize(6);
    tosend.setQuery(2);
    nm.send(tosend);
    return 0;
}
