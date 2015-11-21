#ifndef NETWORKMANAGER_HH_
# define NETWORKMANAGER_HH_

# include <list>
# include <cstdint>
# include "UdpSocket.hh"
# include "TcpSocket.hh"

namespace NetworkState
{
    char tcp = 1;
    char udp = 2;
}

class NetworkManager
{
    public:
        NetworkManager();
        virtual ~NetworkManager();

        void    send(const UdpPacket &packet);
        void    receive();
        UdpPacket  getPacket();

    private:
        std::list<UdpPacket>          _udpPacketList;
        UdpSocket                     _udp;
        TcpSocket                     _tcp;
        char                          _state;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
