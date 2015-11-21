#ifndef NETWORKMANAGER_HH_
# define NETWORKMANAGER_HH_

# include <list>
# include <cstdint>
# include "IPacket.hh"
# include "IUdpSocket.hh"
# include "ITcpSocket.hh"

class NetworkManager
{
    public:
        NetworkManager();
        virtual ~NetworkManager();

        void            receive();
        void            send(const IPacket &packet);
        IPacket         &getPacket();

    private:
        std::list<IPacket*>         _PacketList;
        IUdpSocket                  &_udp;
        ITcpSocket                  &_tcp;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
