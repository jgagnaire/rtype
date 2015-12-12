#ifndef NETWORKMANAGER_HH_
# define NETWORKMANAGER_HH_

# include <list>
# include <cstdint>
# include "IPacket.hh"
# include "IUdpSocket.hh"
# include "ITcpSocket.hh"

enum class Codes
{
    Ping = 109,
    Pong = 110,
    Login = 100,
    Register = 101,
    Ok = 104,
    WrongUserPass = 500,
    AlreadyLogin = 501,
    AlreadyExist = 502,
    WrongFormat = 503
};

class NetworkManager
{
    public:
        NetworkManager(const std::string &ip, const std::string &udpIp);
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
        std::string                 _udpIp;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
