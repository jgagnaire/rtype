#include "GameManager.hh"
#include "GameplayController.hh"

template<typename T>
GameplayController<T>::GameplayController(std::list<UserManager<T> *> *cl)
: AController<T>(*cl) {}

template<typename T>
int	            GameplayController<T>::newData(UserManager<T> *cl) {
    if (!cl->isLogged()) {
        writeStruct({0, Enum::EUDP_NOT_LOGGEDIN, cl->getUDPPacketId()});
        writeOnMe(cl->getIP(), "1726"); // TODO, no magic string
        return (1);
    }
    else if (cl->getStatus() != Enum::GAME_ROOM) {
        writeStruct({0, Enum::ENOT_IN_GAME, cl->getUDPPacketId()});
        writeOnMe(cl->getIP(), "1726"); // TODO, no magic string
        return (1);
    }
    switch (cl->numUDPQuery()) {
        case (Enum::QUIT_GAME):
            return (static_cast<int>(quitGame(cl)));
        case (Enum::CURRENT_POS):
            return (static_cast<int>(currentPosition(cl)));
        case (Enum::KEY_PRESS):
            return (static_cast<int>(keyPressed(cl)));
        case (Enum::AUDIO_PACKET):
            return (static_cast<int>(audioPacket(cl)));
        case (Enum::TAKE_FORCE):
            return (static_cast<int>(takeForce(cl)));
        default:
            return (-1);
    }
}

template <typename T>
bool            GameplayController<T>::quitGame(UserManager<T> *cl) {
    GameManager<T>     &g = GameManager<T>::instance();
    Game<T>           *game = g.getGameByName(cl->getGameroomName());

    if (game) {
        writeStruct({0, cl->quitGame(), cl->getUDPPacketId()});
        writeOnMe(cl->getIP(), "1726"); // TODO, no magic string
        const std::string &u_name = cl->getName();
        for (auto it = game->players.begin(); it != game->players.end(); ++it) {
            if ((*it)->getName() != u_name) {
                writeStruct({static_cast<uint16_t>(u_name.size()),
                             Enum::USER_QUIT_GAME, (*it)->getUDPPacketId()});
                writeMsg(u_name);
                writeOnMe((*it)->getIP(), "1726"); // TODO, no magic string
            }
        }
        g.launchGame(cl->getGameroomName());
    }
    cl->writeStruct({0, cl->quitGame()});
    return (true);
}

template <typename T>
bool            GameplayController<T>::currentPosition(UserManager<T> *cl) {
    writeStruct({0, cl->currentPosition(), cl->getUDPPacketId()});
    writeOnMe(cl->getIP(), "1726"); // TODO, no magic string
    return (true);
}

template <typename T>
bool            GameplayController<T>::keyPressed(UserManager<T> *cl) {
    writeStruct({0, cl->keyPressed(), cl->getUDPPacketId()});
    writeOnMe(cl->getIP(), "1726"); // TODO, no magic string
    return (true);
}

template <typename T>
bool            GameplayController<T>::audioPacket(UserManager<T> *) const {
    return (true);
}

template <typename T>
bool            GameplayController<T>::takeForce(UserManager<T> *cl) {
    writeStruct({0, cl->takeForce(), cl->getUDPPacketId()});
    writeOnMe(cl->getIP(), "1726"); // TODO, no magic string
    return (true);
}

template <typename T>
void	GameplayController<T>::writeStruct(const UDPDataHeader &comdata) {
    packet.stockOnBuff(comdata);
}

template <typename T>
void	GameplayController<T>::writeMsg(const std::string &s) {
    packet.stockOnBuff(s);
}

template <typename T>
bool	GameplayController<T>::writeOnMe(const std::string &ip, const std::string &port) {
    packet.serialize();
    return (packet.sendPacket<IServerSocket<T> *>(this->udp_socket, ip, port));
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
template class GameplayController<SOCKET>;
#else
template class GameplayController<int>;
#endif