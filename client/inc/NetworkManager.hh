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
        NetworkManager(const std::string &ip, unsigned short port,
                const std::string &udpIp, unsigned short updPort);
        virtual ~NetworkManager();

        void            send(const IPacket &packet);
        IPacket         *getPacket();

    private:
        void            receiveUdp();
        void            receiveTcp();

        std::list<IPacket*>         _packets;
        IUdpSocket                  &_udp;
        ITcpSocket                  &_tcp;
        std::string                 _tcpIp;
        unsigned short              _tcpPort;
        std::string                 _udpIp;
        unsigned short              _udpPort;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
