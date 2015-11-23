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

        void            send(const IPacket &packet);
        IPacket         *getPacket();

    private:
        void            receiveUdp();
        void            receiveTcp();

        std::list<IPacket*>         _packets;
        IUdpSocket                  &_udp;
        ITcpSocket                  &_tcp;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
