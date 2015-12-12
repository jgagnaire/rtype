#pragma once

# include <list>
# include <iostream>
# include "UserManager.hh"
# include "ThreadPool.hh"

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#  include <windows.h>
#  define portable_sleep(x) Sleep(x);
# else
#  include <unistd.h>
#  define portable_sleep(x) usleep(x)
# endif

template <typename SCK>
struct Game { // TODO, fill it !
    std::string                                 name;
    std::list<UserManager<SCK> *>               players;
    std::size_t                                 refresh = Enum::REFRESH_TIME;
    bool                                        is_playing = false;
};

template <typename SCK>
class GameManager {
public:
    static  GameManager             &instance();
    static  std::size_t             getTime();
    Game<SCK>                       *getGameByName(const std::string &);
    void                            createRoom(const std::string &, UserManager<SCK> *);
    void                            deleteUser(UserManager<SCK> *);
    bool                            joinRoom(const std::string &, UserManager<SCK> *);
    bool                            roomIsFull(const std::string &);
    bool                            isAllReady(const std::string &);
    const std::list<Game<SCK> *>    &getGames() const;
    void                            launchGame(const std::string &);
    static void                     createGame(Game<SCK> *);
    bool                            isPlaying(const std::string &);
    void                            updatePositions(Game<SCK> *);
    void                            setUdpSocket(IServerSocket<SCK> *);

private:
    static bool                     update(Game<SCK> *game);
    static  GameManager                     *game_manager;
    GameManager() {}
    ~GameManager() {}
    std::list<Game<SCK> *>                  _games;
    ThreadPool<void, Game<SCK> *>           _threadpool;
    IServerSocket<SCK>                      *_udp_socket;
};

