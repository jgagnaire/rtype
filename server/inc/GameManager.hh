#pragma once

#include <list>
#include <iostream>
#include "UserManager.hh"

template <typename SCK>
struct Game { // TODO, fill it !
    std::string                 name;
    std::list<UserManager<SCK> *>                players;
};

template <typename SCK>
class GameManager {
public:
    static  GameManager     &instance();
    static  GameManager     *game_manager;
    Game<SCK>               *getGameByName(const std::string &);
    void                    createRoom(const std::string &, UserManager<SCK> *);
    void                    deleteUser(UserManager<SCK> *);
    bool                    joinRoom(const std::string &, UserManager<SCK> *);
    bool                    roomIsFull(const std::string &);
    const std::list<Game<SCK> *>        &getGames() const;

private:
    GameManager() {}
    ~GameManager() {}
    std::list<Game<SCK> *>       _games;
};

