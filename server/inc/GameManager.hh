#pragma once

#include <list>
#include <iostream>
#include "UserManager.hh"
#include "ThreadPool.hh"

template <typename SCK>
struct Game { // TODO, fill it !
    std::string                 name;
    std::list<UserManager<SCK> *>                players;
};

template <typename SCK>
class GameManager {
public:
    static  GameManager             &instance();
    static  GameManager             *game_manager;
    Game<SCK>                       *getGameByName(const std::string &);
    void                            createRoom(const std::string &, UserManager<SCK> *);
    void                            deleteUser(UserManager<SCK> *);
    bool                            joinRoom(const std::string &, UserManager<SCK> *);
    bool                            roomIsFull(const std::string &);
    bool                            isAllReady(const std::string &);
    const std::list<Game<SCK> *>    &getGames() const;
    void                            launchGame(const std::string &);
    static void                     createGame(Game<SCK> *);

private:
    GameManager() {}
    ~GameManager() {}
    std::list<Game<SCK> *>                  _games;
    ThreadPool<void, Game<SCK> *>           _threadpool;
};

