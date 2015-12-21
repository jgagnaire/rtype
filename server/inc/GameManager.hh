#pragma once

# include <list>
# include <iostream>
# include <unordered_map>
# include "ShootSystem.hh"
# include "MobSystem.hh"
# include "BonusSystem.hh"
# include "JSONParser.hh"
# include "UserManager.hh"
# include "ThreadPool.hh"

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#  include <windows.h>
#  define portable_sleep(x) Sleep(x / 1000);
# else
#  include <unistd.h>
#  define portable_sleep(x) usleep(x)
# endif

template <typename SCK>
struct Game {
  ~Game() {
    if (system["shoot"]) {
      delete system["shoot"];
      delete system["monsters"];
      delete system["bonuses"];
    }
  }

  Game() {
    system["shoot"] = new ShootSystem;
    system["monsters"] = new MobSystem; 
    system["bonuses"] = new BonusSystem;
  }

    std::unordered_map<std::string, ASystem *>  system;
    std::string                                 name;
    std::list<UserManager<SCK> *>               players;
    std::size_t                                 refresh = Enum::REFRESH_TIME;
    bool                                        is_playing = false;
    float					time;
    std::string					lvl_name = "level1";
    std::unordered_map<std::string, Entity>	entities;
    Entity					*level;
    std::unordered_map<std::string, std::string>  content_system;
};

template <typename SCK>
class GameManager {
public:
    static  GameManager             &instance();
    static  std::size_t             getTime();
    static  std::size_t             getTimeInSecond();
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
    void                            setUdpSocket(IServerSocket<SCK> *);
    void			    sendPosition(Game<SCK> *, UserManager<SCK> *);
    void			    fireBall(Game<SCK> *, UserManager<SCK> *, bool);

private:
    bool                            update(Game<SCK> *game, std::size_t);
    void                            updatePositions(Game<SCK> *, std::size_t);
    bool			    updateTime(Game<SCK> *);
    bool		            updateObjSighting(Game<SCK> *game,
						      std::size_t time,
						      const std::string &);
    bool			    checkEntities(Game<SCK> *, 
						  std::pair<std::string, Entity&>,
						  int,
						  std::size_t,
						  const std::string &);

    static  GameManager				  *game_manager;
    static  Entity				  configuration;
    std::list<Game<SCK> *>			  _games;
    ThreadPool<void, Game<SCK> *>		  _threadpool;
    IServerSocket<SCK>				  *_udp_socket;
    std::unordered_map<std::string, JSONParser *> _game_system;

    GameManager();
    ~GameManager() {}
};

