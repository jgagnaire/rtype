#include <algorithm>
#include <sys/timeb.h>
#include <GameManager.hh>
#include "UserManager.hh"

template<typename T>
const std::string	UserManager<T>::database_dir = "./server/.database/.%_data";

template<typename T>
# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
IMutex		*UserManager<T>::user_mutex = new WinMutex;
# else
IMutex		*UserManager<T>::user_mutex = new UnixMutex;
# endif

template<typename T>
UserManager<T>::UserManager(IServerSocket<T> *sck) :
        sock(sck) , status(Enum::LOBBY) , ping(true) {
# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
    game_mutex = new WinMutex;
    destroy_client_mutex = new WinMutex;
# else
    game_mutex = new UnixMutex;
    destroy_client_mutex = new UnixMutex;
# endif
    clearData();
}

template<typename T>
UserManager<T>::~UserManager() {
    GameManager<T>             &gm = GameManager<T>::instance();

    destroy_client_mutex->lock();
    gm.deleteUser(this);
    if (stream.is_open())
        stream.close();
    destroy_client_mutex->unlock();
    delete this->sock;
    delete this->destroy_client_mutex;
    delete this->game_mutex;
}

template<typename T>
inline
bool	UserManager<T>::isLogged() const { return (stream.is_open()); }

template<typename T>
inline
void	UserManager<T>::writeStruct(const TCPDataHeader &comdata) { packet.stockOnBuff(comdata); }

template<typename T>
inline
void	UserManager<T>::readFromMe() { packet.getPacket<IServerSocket<T> *>(sock); }

template<typename T>
inline
void	UserManager<T>::writeMsg(const std::string &s) { packet.stockOnBuff(s); }

template<typename T>
inline
bool	UserManager<T>::writeOnMe()
{
    packet.serialize();
    return (packet.sendPacket<IServerSocket<T> *>(sock));
}

template<typename T>
inline
bool	UserManager<T>::sendStructEmpty() const { return (packet.sendPackEmpty()); }

template<typename T>
inline
IServerSocket<T>	*UserManager<T>::getServerSocket() { return (this->sock); }

template<typename T>
inline
bool	UserManager<T>::IsFilled() const { return (packet.isFilled()); }

template<typename T>
inline
void	UserManager<T>::clearData() { packet.clearAll(); }

template<typename T>
inline
void	UserManager<T>::fillPacketStruct() { tmp_packet = packet.retrievePacket(); }

template<typename T>
inline
bool	UserManager<T>::emptyData() const {
    return (packet.packetEmpty());
}

template<typename T>
inline
Enum::ClientQueries	UserManager<T>::numQuery() const {
    return (static_cast<Enum::ClientQueries>(tmp_packet.header.query));
}

template<typename T>
inline
Enum::ClientQueries	UserManager<T>::numUDPQuery() const {
    return (static_cast<Enum::ClientQueries>(udp_packet.header.query));
}

template<typename T>
inline
std::string	UserManager<T>::getPacketData() const { return (std::string(tmp_packet.data)); }

template<typename T>
inline
std::string	UserManager<T>::getUDPPacketData() const {return (std::string(udp_packet.data)); }

template<typename T>
inline
std::string	UserManager<T>::getIP() const { return (this->sock->getIP()); }

template<typename T>
Enum::ServerAnswers	UserManager<T>::verifyUser() {
    std::string s(tmp_packet.data);
    std::string tok;
    std::istringstream is(s);
    std::string	pass;
    std::string	tmp_log;

    std::getline(is, tok, ':');
    if (stream.is_open())
        return (Enum::EALREADY_LOGGED);
    stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
                                                                  1, tok),
                std::fstream::in | std::fstream::out);
    stream.seekg(0, stream.beg);
    if (!stream.is_open())
        return (Enum::EUSERPASS);
    std::getline(is, pass, ':');
    tmp_log = tok + ":" + pass;
    if (tmp_log == s) {
        name = tok;
        stream.clear();
        stream.seekg(0, stream.end);
        return (Enum::OK);
    }
    stream.close();
    return (Enum::EUSERPASS);
}

template<typename T>
Enum::ServerAnswers	UserManager<T>::newUser() {
    std::string s(tmp_packet.data);
    std::istringstream is(s);
    std::string tok[2];

    if (stream.is_open())
        return (Enum::EALREADY_LOGGED);
    std::getline(is, tok[0], ':');
    std::getline(is, tok[1], ':');
    if (alreadyExist(tok))
    {
        stream.close();
        return (Enum::EUSER_EXIST);
    }
    if (hasBadFormat(tok))
        return (Enum::EREGISTER_FORMAT);
    stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
                                                                  1, tok[0]).c_str(),
                std::fstream::in |
                std::fstream::out |
                std::fstream::app);
    if (!stream.is_open())
        return (Enum::EUSER_EXIST);
    stream.clear();
    stream << tok[0] << ":" << tok[1] << std::flush;
    name = tok[0];
    stream.close();
    return (Enum::OK);
}

template<typename T>
bool	UserManager<T>::hasBadFormat(std::string *info) const {
    for (int i = 0; i < 2; ++i) {
        auto it = std::find_if_not(info[i].begin(), info[i].end(), [](char c) {
            return ((c >= 'a' && c <= 'z') ||
                    (c >= 'A' && c <= 'Z') ||
                    (c >= '0' && c <= '9') ||
                    (c == '-' || c == '_'));
        });
        if (it != info[i].end())
            return (true);
    }
    return (((info[0].size() < Enum::REGISTER_MIN) || (info[0].size() > Enum::REGISTER_MAX)) ||
            ((info[1].size() < Enum::REGISTER_MIN) || (info[1].size() > Enum::REGISTER_MAX)));
}

template<typename T>
inline
bool			UserManager<T>::alreadyExist(std::string *info) {
    stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
                                                                  1, info[0]),
                std::fstream::in | std::fstream::out);
    return (stream.is_open());
}

template<typename T>
inline
void	UserManager<T>::sendPing() { writeStruct({0, Enum::PING}); }

template<typename T>
inline
void	UserManager<T>::setPing(bool p) { ping = p; }

template<typename T>
inline
bool	UserManager<T>::getPing() const { return (ping); }

template<typename T>
inline
void    UserManager<T>::inGame() { status = Enum::GAME; }

template<typename T>
void	UserManager<T>::disconnect() {
    if (!stream.is_open())
        return ;
    stream.close();
    name.clear();
}

template<typename T>
inline
const std::string	&UserManager<T>::getName() const { return (name); }

template <typename T>
inline
void    UserManager<T>::setUdpPacketStruct(const Packet<UDPDataHeader>::PacketStruct &p) {
    udp_packet = p;
}

template <typename T>
Enum::ServerAnswers      UserManager<T>::joinRandomRoom() { //TODO, implements
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    is_ready = false;
    return (Enum::OK);
}

template <typename T>
Enum::ServerAnswers      UserManager<T>::joinNamedRoom() {
    std::string             game_name = getPacketData();
    GameManager<T>             &gm = GameManager<T>::instance();

    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (!gameroom.empty())
        return (Enum::EALREADY_ON_ROOM);
    if (gm.roomIsFull(game_name))
        return (Enum::ENO_AVAILABLE_ROOM);
    else if (!gm.joinRoom(game_name, this))
        return (Enum::EROOM_NO_EXIST);
    gameroom = game_name;
    status = Enum::GAME_ROOM;
    is_ready = false;
    return (Enum::OK);
}

template <typename T>
std::string     UserManager<T>::generateRoomName() {
    static const char alphanum[] =
            "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";

    std::string s;
    std::srand(static_cast<unsigned int>(GameManager<T>::getTime()) *
               Enum::MAX_ROOM_NAME_NB);
    for (int i = 0; i < Enum::MAX_ROOM_NAME_NB; ++i)
        s.push_back(alphanum[std::rand() % (sizeof(alphanum) - 1)]);
    return s;

}

template <typename T>
Enum::ServerAnswers      UserManager<T>::createGameRoom() {
    std::string             game_name = getPacketData();
    GameManager<T>          &gm = GameManager<T>::instance();

    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (!gameroom.empty())
        return (Enum::EALREADY_ON_ROOM);
    if (game_name.empty())
        game_name = generateRoomName();
    is_ready = false;
    gameroom = game_name;
    std::cout << "La room: " << gameroom << " se cree "<< std::endl;
    gm.createRoom(game_name, this);
    std::cout << "il y a donc " << gm.getGames().size() << " rooms" << std::endl;
    status = Enum::GAME_ROOM;
    return (Enum::OK);
}

template <typename T>
Enum::ServerAnswers      UserManager<T>::leaveRoom() {
    GameManager<T>             &gm = GameManager<T>::instance();

    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (gameroom.empty())
        return (Enum::ENOT_IN_ROOM);
    is_ready = false;
    gm.deleteUser(this);
    gameroom.clear();
    status = Enum::LOBBY;
    return (Enum::OK);
}

template <typename T>
Enum::ServerAnswers      UserManager<T>::ready() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (gameroom.empty())
        return (Enum::ENOT_IN_ROOM);
    if (status == Enum::GAME)
      return (Enum::EALREADY_IN_GAME);
    is_ready = true;
    return (Enum::OK);
}

template <typename T>
inline
bool                    UserManager<T>::isFiring() { return (fire); }

template <typename T>
void                    UserManager<T>::clearGameData() {
    keypressed = 0;
    has_force = 0;
    position.clear();
    has_force = false;
    udp_packet_id = 0;
    fire = false;
}

template <typename T>
inline
uint64_t                UserManager<T>::getUDPPacketId() { return (udp_packet_id++); }

template <typename T>
Enum::ServerAnswers      UserManager<T>::notReady() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (status == Enum::GAME)
      return (Enum::EALREADY_IN_GAME);
    if (gameroom.empty())
        return (Enum::ENOT_IN_ROOM);
    is_ready = false;
    return (Enum::OK);
}

template <typename T>
Enum::ServerAnswers      UserManager<T>::getRoomList() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (!gameroom.empty())
        return (Enum::EALREADY_ON_ROOM);
    return (Enum::OK);
}

template <typename T>
inline
bool                    UserManager<T>::isReady() const { return (is_ready); }

template <typename T>
inline
Enum::UserStatus        UserManager<T>::getStatus() const { return (status); }

template <typename T>
inline
const std::string       &UserManager<T>::getGameroomName() const { return (gameroom); }

template <typename T>
inline
Enum::ServerAnswers     UserManager<T>::quitGame() { return (Enum::OK); }

template <typename T>
Enum::ServerAnswers     UserManager<T>::currentPosition() {
    game_mutex->lock();
    std::istringstream  is(udp_packet.data);
    std::istringstream  is_pos;
    std::string         pos[2];


    for (int i = 0; i < 2; ++i)
        getline(is, pos[i], ':');
    is_pos.str(pos[0]);
    is_pos >> position.x;
    is_pos.clear();
    is_pos.str("");
    is_pos.str(pos[1]);
    is_pos >> position.y;
    game_mutex->unlock();
    return (Enum::OK);
}

template <typename T>
Enum::ServerAnswers     UserManager<T>::keyPressed() {
    game_mutex->lock();
    std::istringstream  is(udp_packet.data);

    is >> keypressed;
    game_mutex->unlock();
    return (Enum::OK);
}

template <typename T>
inline
const std::size_t	    &UserManager<T>::getKeypressed() const { return (keypressed); }

template <typename T>
void                    UserManager<T>::changePosition(std::size_t time) {
    game_mutex->lock();
    // float k = 1.0f;
    // for (unsigned i = 1; i < sizeof(uint64_t) * 8; i <<= 1) {
    //   if (keypressed & i)
    // 	++k;
    //   if (k >= 2)
    // 	break ;
    // }
    float	move = static_cast<float>(time * 1.75);
    if (static_cast<std::size_t>(Enum::LEFT) & keypressed) {
        position.x -= move;
        if (position.x < 0.01)
            position.x = 0;
    }
    if (static_cast<std::size_t>(Enum::RIGHT) & keypressed) {
        position.x += move;
        if (position.x > static_cast<float>(Enum::GAME_SIZE_WIDTH))
            position.x = static_cast<float>(Enum::GAME_SIZE_WIDTH);
    }
    if (static_cast<std::size_t>(Enum::UP) & keypressed) {
        position.y -= move;
        if (position.y < 0.01)
            position.y = 0;
    }
    if (static_cast<std::size_t>(Enum::DOWN) & keypressed) {
        position.y += move;
        if (position.y > static_cast<float>(Enum::GAME_SIZE_HEIGHT))
            position.y = static_cast<float>(Enum::GAME_SIZE_HEIGHT);
    }
    fire = !(static_cast<std::size_t>(Enum::FIRE) & keypressed);

    GameManager<T>	&g = GameManager<T>::instance();

    if (keypressed)
      g.sendPosition(g.getGameByName(gameroom), this);
    keypressed = 0;
    game_mutex->unlock();
}

template <typename T>
inline
Enum::ServerAnswers     UserManager<T>::takeForce() {
    has_force = true;
    return (Enum::OK);
}

template <typename T>
inline
const Position	    &UserManager<T>::getPosition() const { return (position); }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
template class UserManager<SOCKET>;
#else
template class UserManager<int>;
#endif

