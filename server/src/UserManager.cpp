#include <algorithm>
#include <sys/timeb.h>
#include <GameManager.hh>
#include "UserManager.hh"

template<typename T>
const std::string	UserManager<T>::database_dir = "./server/.database/.%_data";

template<typename T>
std::mutex          UserManager<T>::user_mutex;

template<typename T>
UserManager<T>::UserManager(IServerSocket<T> *sck) :
  sock(sck) , status(Enum::LOBBY) , ping(true) {
    clearData();
    latency[0] = 0;
    latency[1] = 0;
    latency[2] = 0;
    latency[3] = 0;
}

template<typename T>
UserManager<T>::~UserManager() {
    GameManager<T>             &gm = GameManager<T>::instance();

    destroy_client_mutex.lock();
    gm.deleteUser(this);
    if (stream.is_open())
        stream.close();
    destroy_client_mutex.unlock();
}

template<typename T>
bool	UserManager<T>::isRespawning() const { return (respawn > 0); }

template<typename T>
bool	UserManager<T>::isLogged() const { return (stream.is_open()); }

template<typename T>
void	UserManager<T>::writeStruct(const TCPDataHeader &comdata) { packet.stockOnBuff(comdata); }

template<typename T>
void	UserManager<T>::readFromMe() { packet.getPacket<IServerSocket<T> *>(sock); }

template<typename T>
void	UserManager<T>::writeMsg(const std::string &s) { packet.stockOnBuff(s); }

template<typename T>
Enum::ServerAnswers	UserManager<T>::setLatency() {
  if (!latency[1]) {
    latency[1] = GameManager<T>::getTime();
    return (Enum::GET_LAT);
  }
  if (!latency[1]) {
    latency[1] = GameManager<T>::getTime();
    return (Enum::GET_LAT);
  }
  if (!latency[2]) {
    latency[2] = GameManager<T>::getTime();
    return (Enum::GET_LAT);
  }
  if (!latency[3]) {
    latency[3] = GameManager<T>::getTime();
    return (Enum::GET_LAT);
  }
  return (Enum::OK);
}

template<typename T>
const std::size_t    *UserManager<T>::getLatency() { return (latency); }

template<typename T>
bool	UserManager<T>::writeOnMe()
{
    packet.serialize();
    return (packet.sendPacket<IServerSocket<T> *>(sock));
}

template<typename T>
bool	UserManager<T>::sendStructEmpty() const { return (packet.sendPackEmpty()); }

template<typename T>
void	UserManager<T>::getBonus(Entity *bonus) {
  if (bonus->manager.exist<int>("duration"))
    perfect_shield = bonus->manager.get<int>("duration") * 1000;
  else if (bonus->manager.exist<int>("power"))
    force = bonus->manager.get<int>("power");
  else if (bonus->manager.exist<int>("protection"))
    protection = bonus->manager.get<int>("protection");
}

template<typename T>
IServerSocket<T>	*UserManager<T>::getServerSocket() { return (this->sock); }

template<typename T>
bool	UserManager<T>::IsFilled() const { return (packet.isFilled()); }

template<typename T>
void	UserManager<T>::clearData() { 
  packet.clearAll();
  init_memory(reinterpret_cast<char *>(&tmp_packet), sizeof(TCPDataHeader));
  init_memory(reinterpret_cast<char *>(&udp_packet), sizeof(UDPDataHeader));
}

template<typename T>
void	UserManager<T>::fillPacketStruct() { tmp_packet = packet.retrievePacket(); }

template<typename T>
bool	UserManager<T>::emptyData() const {
    return (packet.packetEmpty());
}

template<typename T>
Enum::ClientQueries	UserManager<T>::numQuery() const {
    return (static_cast<Enum::ClientQueries>(tmp_packet.header.query));
}

template<typename T>
Enum::ClientQueries	UserManager<T>::numUDPQuery() const {
    return (static_cast<Enum::ClientQueries>(udp_packet.header.query));
}

template<typename T>
std::string	UserManager<T>::getPacketData() const { return (std::string(tmp_packet.data)); }

template<typename T>
std::string	UserManager<T>::getUDPPacketData() const {return (std::string(udp_packet.data)); }

template<typename T>
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
    std::getline(stream, tmp_log, '\n');
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
bool			UserManager<T>::alreadyExist(std::string *info) {
    stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
                                                                  1, info[0]),
                std::fstream::in | std::fstream::out);
    return (stream.is_open());
}

template<typename T>
void	UserManager<T>::sendPing() { writeStruct({0, Enum::PING}); }

template<typename T>
void	UserManager<T>::setPing(bool p) { ping = p; }

template<typename T>
bool	UserManager<T>::getPing() const { return (ping); }

template<typename T>
void    UserManager<T>::inGame() { status = Enum::GAME; }

template<typename T>
void	UserManager<T>::disconnect() {
    if (!stream.is_open())
        return ;
    stream.close();
    name.clear();
}

template<typename T>
const std::string	&UserManager<T>::getName() const { return (name); }

template <typename T>
void    UserManager<T>::setUdpPacketStruct(const Packet<UDPDataHeader>::PacketStruct &p) {
    udp_packet = p;
}

template <typename T>
void    UserManager<T>::setUdpBinaryPacketStruct(UDPData &p) { udp_binary_packet = p; }

template <typename T>
const UDPData    &UserManager<T>::getUdpBinaryPacketStruct() const { 
  return (udp_binary_packet);
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
    latency[0] = GameManager<T>::getTime();
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
    Game<T> *g = gm.getGameByName(game_name);
    if (g)
      id = g->getId();
    status = Enum::GAME_ROOM;
    latency[0] = GameManager<T>::getTime();
    return (Enum::OK);
}

template <typename T>
void      UserManager<T>::onGameRoom() {
  status = Enum::GAME_ROOM;
  is_ready = false;
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
bool                    UserManager<T>::isFiring() const { return (fire); }

template <typename T>
void                    UserManager<T>::setId(std::size_t i) { id = i; }
template <typename T>
std::size_t		UserManager<T>::getId() { return (id); }

template <typename T>
bool                    UserManager<T>::isDead() const { return (life <= 0); }

template <typename T>
void                    UserManager<T>::isTouched(int damage) {
  GameManager<T>	&g = GameManager<T>::instance();

  if (perfect_shield > 0)
    return ;
  if (protection > 0) {
    protection -= damage;
    return ;
  }
  perfect_shield = 0;
  protection = 0;
  force = 0;
  position.x = 0;
  position.y = Enum::GAME_SIZE_HEIGHT / 2;
  respawn = Enum::RESPAWN_TIME;
  --life;
  g.sendPosition(g.getGameByName(gameroom), this);
}

template <typename T>
void                    UserManager<T>::clearLevel() {
    GameManager<T>          &g = GameManager<T>::instance();

    keypressed = 0;
    has_force = 0;
    position.clear();
    has_force = false;
    fire = false;
    switch_weapon = false;
    is_dead = false;
    position.x = 0;
    position.y = Enum::GAME_SIZE_HEIGHT / 2;
    force = 0;
    protection = 0;
    perfect_shield = 0;
    respawn = Enum::RESPAWN_TIME;
    g.sendPosition(g.getGameByName(gameroom), this);
}

template <typename T>
void                    UserManager<T>::clearGameData() {
    keypressed = 0;
    has_force = 0;
    position.clear();
    has_force = false;
    udp_packet_id = 0;
    fire = false;
    switch_weapon = false;
    is_dead = false;
    life = 10;
    position.x = 0;
    position.y = Enum::GAME_SIZE_HEIGHT / 2;
    force = 0;
    protection = 0;
    perfect_shield = 0;
    respawn = Enum::RESPAWN_TIME;
}

template <typename T>
bool                UserManager<T>::updateBonus(std::size_t duration) {
  if (perfect_shield > 0)
    perfect_shield -= duration;
  if (respawn > 0)
    respawn -= duration;
  return (true);
}

template <typename T>
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
bool                    UserManager<T>::isReady() const { return (is_ready); }

template <typename T>
Enum::UserStatus        UserManager<T>::getStatus() const { return (status); }

template <typename T>
const std::string       &UserManager<T>::getGameroomName() const { return (gameroom); }

template <typename T>
Enum::ServerAnswers     UserManager<T>::quitGame() { return (Enum::OK); }

template <typename T>
Enum::ServerAnswers     UserManager<T>::currentPosition() {
    game_mutex.lock();
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
    game_mutex.unlock();
    return (Enum::OK);
}

template <typename T>
Enum::ServerAnswers     UserManager<T>::keyPressed() {
    game_mutex.lock();
    std::istringstream  is(udp_packet.data);

    is >> keypressed;
    game_mutex.unlock();
    return (Enum::OK);
}

template <typename T>
const std::size_t	    &UserManager<T>::getKeypressed() const { return (keypressed); }

template <typename T>
void                    UserManager<T>::changePosition(std::size_t time) {
    game_mutex.lock();
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
    fire = !!(static_cast<std::size_t>(Enum::FIRE) & keypressed);
    if (static_cast<std::size_t>(Enum::SWITCH) & keypressed)
      switch_weapon = !switch_weapon; 

    GameManager<T>	&g = GameManager<T>::instance();
    Game<T>		*game = g.getGameByName(gameroom);

    if (keypressed)
      g.sendPosition(game, this);
    if (fire)
      g.fireBall(game, this, switch_weapon);
    keypressed = 0;
    fire = false;
    game_mutex.unlock();
}

template <typename T>
Enum::ServerAnswers     UserManager<T>::takeForce() {
    has_force = true;
    return (Enum::OK);
}

template <typename T>
const Position	    &UserManager<T>::getPosition() const { return (position); }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
template class UserManager<SOCKET>;
#else
template class UserManager<int>;
#endif

