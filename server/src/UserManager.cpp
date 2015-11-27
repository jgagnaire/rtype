#include <algorithm>
#include <GameManager.hh>
#include "UserManager.hh"

template<typename T>
const std::string UserManager<T>::database_dir = "./server/.database/.%_data";

template<typename T>
UserManager<T>::UserManager(IServerSocket<T> *sck) :
        sock(sck) , status(Enum::LOBBY) { clearData(); }

template<typename T>
UserManager<T>::~UserManager() {
    if (image_stream.is_open())
        image_stream.close();
    if (stream.is_open())
        stream.close();
    delete this->sock;
}

template<typename T>
bool	UserManager<T>::isLogged() const { return (stream.is_open()); }

template<typename T>
void	UserManager<T>::writeStruct(const ::TCPDataHeader &comdata) {
    packet.stockOnBuff(comdata);
}

template<typename T>
void	UserManager<T>::readFromMe() {
    packet.getPacket<IServerSocket<T> *>(sock);
}

template<typename T>
void	UserManager<T>::writeMsg(const std::string &s) {
    packet.stockOnBuff(s);
}

template<typename T>
bool	UserManager<T>::writeOnMe()
{
    packet.serialize();
    return (packet.sendPacket<IServerSocket<T> *>(sock));
}

template<typename T>
bool	UserManager<T>::sendStructEmpty() const { return (packet.sendPackEmpty()); }

template<typename T>
IServerSocket<T>	*UserManager<T>::getServerSocket() { return (this->sock); }

template<typename T>
bool	UserManager<T>::IsFilled() const {
    return (packet.isFilled());
}

template<typename T>
void	UserManager<T>::clearData() {
    packet.clearAll();
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
std::string	UserManager<T>::getPacketData() const { return (std::string(tmp_packet.data)); }

template<typename T>
std::string	UserManager<T>::getIP() const { return (this->sock->getIP()); }

template<typename T>
Enum::TCPServerAnswers	UserManager<T>::verifyUser() {
    std::string s(tmp_packet.data);
    std::string tmp;
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
    std::getline(stream, tmp, '|');
    std::getline(is, pass, ':');
    tmp_log = tok + ":" + pass;
    if (!tmp.compare(0, tmp_log.size(), tmp_log)
        && (tmp.size() - 2 == tmp_log.size())) {
        name = tok;
        stream.clear();
        stream.seekg(0, stream.end);
        return (Enum::LOGGED_IN);
    }
    stream.close();
    return (Enum::EUSERPASS);
}

template<typename T>
std::string	UserManager<T>::getFriendName() {
    std::istringstream	is;
    std::string	s;

    if (!stream.is_open())
        return ("");
    std::getline(stream, s, '|');
    return (s);
}

template<typename T>
Enum::TCPServerAnswers	UserManager<T>::newUser() {
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
void	UserManager<T>::disconnect() {
    if (!stream.is_open())
        return ;
    stream.close();
    name.clear();
}

template<typename T>
bool	UserManager<T>::deleteUserWith(const std::string &s,
                                       std::fstream &file,
                                       std::string &stock) {
    std::string	        tmp;
    bool			      found = false;

    stock.clear();
    file.clear();
    file.seekg(0, file.beg);
    std::getline(file, tmp, '|');
    stock = tmp + "|";
    do {
        file.clear();
        std::getline(file, tmp, '|');
        if (tmp == s)
            found = true;
        else {
            if (!tmp.empty())
                stock += tmp + "|";
        }
    } while (!file.eof());
    file.close();
    return (found);
}

template<typename T>
const std::string	&UserManager<T>::getName() const { return (name); }

template <typename T>
Enum::TCPServerAnswers UserManager<T>::startUpload() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (image_stream.is_open())
        return (Enum::EIMAGE_KO);
    image_stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
                                                                        1, name + "_image"),
                      std::fstream::trunc | std::fstream::out);
    return (Enum::OK);
}
template <typename T>
Enum::TCPServerAnswers     UserManager<T>::uploadImage() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (!image_stream.is_open())
        return (Enum::EIMAGE_KO);
    image_stream.clear();
    image_stream << getPacketData() << std::flush;
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers     UserManager<T>::endOfUploading() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (!image_stream.is_open())
        return (Enum::EIMAGE_KO);
    image_stream.close();
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers     UserManager<T>::deleteImage() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (image_stream.is_open())
        return (Enum::EIMAGE_KO);
    image_stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
                                                                        1, name + "_image"),
                      std::fstream::trunc);
    if (!image_stream.is_open())
        return (Enum::EIMAGE_NO_EXIST);
    image_stream.close();
#if (defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64))
    DeleteFile(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
                                                          1, name + "_image").c_str());
#else
    remove(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
                                                             1, name + "_image").c_str());
#endif
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers     UserManager<T>::retrieveImage() { // TODO, implements
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers      UserManager<T>::joinRandomRoom() { //TODO, implements
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    is_ready = false;
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers      UserManager<T>::joinNamedRoom() {
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
Enum::TCPServerAnswers      UserManager<T>::createGameRoom() {
    std::string             game_name = getPacketData();
    GameManager<T>          &gm = GameManager<T>::instance();

    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (!gameroom.empty())
        return (Enum::EALREADY_ON_ROOM);
    if (game_name.empty())
        game_name = "qweqwe"; //TODO, for the moment
    is_ready = false;
    gameroom = game_name;
    gm.createRoom(game_name, this);
    status = Enum::GAME_ROOM;
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers      UserManager<T>::leaveRoom() {
    GameManager<T>             &gm = GameManager<T>::instance();

    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (gameroom.empty())
        return (Enum::ENOT_IN_ROOM);
    gm.deleteUser(this);
    gameroom.clear();
    status = Enum::LOBBY;
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers      UserManager<T>::ready() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (gameroom.empty())
        return (Enum::ENOT_IN_ROOM);
    is_ready = true;
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers      UserManager<T>::notReady() {
    if (!stream.is_open())
        return (Enum::ENOT_LOGGED);
    if (gameroom.empty())
        return (Enum::ENOT_IN_ROOM);
    is_ready = false;
    return (Enum::OK);
}

template <typename T>
Enum::TCPServerAnswers      UserManager<T>::getRoomList() {
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
const std::string      &UserManager<T>::getGameroomName() const { return (gameroom); }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
template class UserManager<SOCKET>;
#else
template class UserManager<int>;
#endif

