#ifndef NETWORKMANAGER_HH_
# define NETWORKMANAGER_HH_

# include <vector>
# include <cstdint>
# include "IPacket.hh"
# include "IUdpSocket.hh"
# include "ITcpSocket.hh"

enum class Codes
{
    nothing = 0,
    Ping = 109,
    Pong = 110,
    Login = 100,
    Register = 101,
    Ok = 104,
    WrongUserPass = 500,
    AlreadyLogin = 501,
    AlreadyExist = 502,
    WrongFormat = 503,
    GetGameRooms = 103,
    AlreadyInRoom = 504,
    NotLoggedIn = 505,
    ExistingRoom = 403,
    CreateRoom = 202,
    JoinRoom = 201,
    RandomRoom = 200,
    LeaveRoom = 300,
    Ready = 301,
    NotReady = 302,
    Begin = 402,
    PlayerJoined = 400,
    PlayerLeft = 401,
};

enum class UdpCodes
{
    KeyPressed = 301,
		ServeKeyPressed = 407,
    NewPos = 408
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

        std::vector<IPacket*>       _packets;
        IUdpSocket                  *_udp;
        ITcpSocket                  *_tcp;
        std::string                 _tcpIp;
        std::string                 _udpIp;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
