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
        NetworkManager(const std::string &ip, unsigned short port);
        virtual ~NetworkManager();

        void            send(const IPacket &packet);
        IPacket         *getPacket();

    private:
        void            receiveUdp();
        void            receiveTcp();

        std::list<IPacket*>         _packets;
        IUdpSocket                  &_udp;
        ITcpSocket                  &_tcp;
        const std::string           _tcpIp;
        unsigned short              _tcpPort;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
