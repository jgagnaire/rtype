#include <chrono>
#include "GameManager.hh"

template <typename SCK>
GameManager<SCK> *GameManager<SCK>::game_manager = 0;

template <typename SCK>
inline
GameManager<SCK>     &GameManager<SCK>::instance() {
    if (GameManager::game_manager == 0)
        GameManager::game_manager = new GameManager();
    return (*game_manager);
}

template <typename SCK>
Game<SCK>        *GameManager<SCK>::getGameByName(const std::string &name) {
    for (auto it = _games.begin(); it != _games.end(); ++it) {
        if ((*it)->name == name)
            return (*it);
    }
    return 0;
}

template <typename SCK>
inline
void        GameManager<SCK>::createRoom(const std::string &name, UserManager<SCK> *s) {
    Game<SCK>    *g = new Game<SCK>;

    g->name = name;
    g->players.push_back(s);
    _games.push_back(g);
}

template <typename SCK>
void        GameManager<SCK>::deleteUser(UserManager<SCK> *u) {
    Game<SCK> *g = getGameByName(u->getGameroomName());

    if (!g)
        return ;
    for (auto it = g->players.begin(); it != g->players.end(); ++it) {
        if (u->getName() == (*it)->getName()) {
            g->players.erase(it);
	    std::cout << u->getName() << " trouve dans la room " << g->name << std::endl;
            break ;
        }
    }
    if (g->players.empty()) {
        for (auto it = _games.begin(); it != _games.end(); ++it) {
            if (g->name == (*it)->name) {
                _games.erase(it);
                break ;
            }
        }
    }
}

template <typename SCK>
bool        GameManager<SCK>::joinRoom(const std::string &name, UserManager<SCK> *u) {
    if (roomIsFull(name))
        return (false);
    Game<SCK> *game = getGameByName(name);

    if (!game)
        return false;
    game->players.push_back(u);
    return true;
}

template <typename SCK>
inline
bool        GameManager<SCK>::roomIsFull(const std::string &name) {
    Game<SCK> *game = getGameByName(name);

    if (!game)
        return false;
    return (game->players.size() == 4);
}

template <typename SCK>
inline
void        GameManager<SCK>::launchGame(const std::string &game_name) {
    Game<SCK>    *game = getGameByName(game_name);

    if (game && game->is_playing) {
        game->is_playing = true;
        _threadpool.add(&GameManager::createGame, game);
    }
}

template <typename SCK>
inline
void        GameManager<SCK>::updatePositions(Game<SCK> *game, std::size_t time) {
    for (auto it = game->players.begin(); it != game->players.end(); ++it)
        (*it)->changePosition(time);
}

template <typename SCK>
inline
bool        GameManager<SCK>::update(Game<SCK> *game, std::size_t time) {
    updatePositions(game, time);
    return (true);
}

template <typename SCK>
inline
std::size_t   GameManager<SCK>::getTime() {
    return (std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1));
}

template <typename SCK>
void            GameManager<SCK>::createGame(Game<SCK> *game) {
    GameManager<SCK> &g = GameManager<SCK>::instance();
    bool        is_not_finished = true;
    std::size_t time = GameManager<SCK>::getTime() - Enum::REFRESH_TIME;

    std::cout << "Que la partie commence pour la room: " << game->name << std::endl;
    while (is_not_finished) {
        if (GameManager<SCK>::getTime() - time >= Enum::REFRESH_TIME) {
            time = GameManager<SCK>::getTime();
            is_not_finished = g.update(game, time);
        }
    }
    game->is_playing = false;
}

template <typename SCK>
inline
bool        GameManager<SCK>::isPlaying(const std::string &roomname) {
    Game<SCK> *g = getGameByName(roomname);

    return (g && g->is_playing);
}

template <typename SCK>
inline
void        GameManager<SCK>::setUdpSocket(IServerSocket<SCK> *sock) { _udp_socket = sock; }

template <typename SCK>
bool        GameManager<SCK>::isAllReady(const std::string &roomname) {
    Game<SCK> *g = getGameByName(roomname);

    if (!g)
        return (false);
    for (auto it = g->players.begin(); it != g->players.end(); ++it) {
        if (!(*it)->isReady())
            return (false);
    }
    return (true);
}

template <typename SCK>
inline
const std::list<Game<SCK> *>        &GameManager<SCK>::getGames() const { return (_games); }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
template class GameManager<SOCKET>;
template struct Game<SOCKET>;
#else
template class GameManager<int>;
template class Game<int>;
#endif
