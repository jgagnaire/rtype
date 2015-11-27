#include "GameManager.hh"

template <typename SCK>
GameManager<SCK> *GameManager<SCK>::game_manager = 0;

template <typename SCK>
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
void        GameManager<SCK>::createRoom(const std::string &name, UserManager<SCK> *s) {
    Game<SCK>    *g = new Game<SCK>;

    g->name = name;
    g->players.push_back(s);
}

template <typename SCK>
void        GameManager<SCK>::deleteUser(UserManager<SCK> *u) {
    Game<SCK> *g = getGameByName(u->getGameroomName());

    if (!g)
        return ;
    for (auto it = g->players.begin(); it != g->players.end(); ++it) {
        if (u->getName() == (*it)->getName()) {
            g->players.erase(it);
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
}

template <typename SCK>
bool        GameManager<SCK>::roomIsFull(const std::string &name) {
    Game<SCK> *game = getGameByName(name);

    if (!game)
        return false;
    return (game->players.size() == 4);
}

template <typename SCK>
const std::list<Game<SCK> *>        &GameManager<SCK>::getGames() const { return (_games); }

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
template class GameManager<SOCKET>;
template class Game<SOCKET>;
#else
template class GameManager<int>;
template class Game<int>;
#endif