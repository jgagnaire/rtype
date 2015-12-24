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
    PingLatency = 111,
    PongLatency = 112,
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
    JsonLevels = 405,
    JsonBonuses = 406,
    JsonShoots = 407,
    JsonMonsters = 408,
    JsonHitboxes = 409,
    GameEnded = 404,
};

enum class UdpCodes
{
  KeyPressed = 301,
    Collided = 401,
    BossDeath = 403,
    AudioPacket = 404,
    PlayerQuit = 406,
    ServeKeyPressed = 407,
    NewPos = 408,
    CodeSendPacket = 302,
    CodeReceivePacket = 404
};

class NetworkManager
{
    public:
        NetworkManager(const std::string &ip, const std::string &udpIp);
        ~NetworkManager();

        void            send(const IPacket &packet);
        IPacket         *getPacket();

        bool            isConnected();
    private:
        void            receiveUdp();
        void            receiveTcp();

        std::vector<IPacket*>       _packets;
        IUdpSocket                  *_udp;
        ITcpSocket                  *_tcp;
        std::string                 _tcpIp;
        std::string                 _udpIp;
        bool                        _connected;
};

#endif /* ifndef NETWORKMANAGER_HH_ */
