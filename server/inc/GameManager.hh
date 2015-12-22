#pragma once

# include <list>
# include <iostream>
# include <unordered_map>
# include "ShootSystem.hh"
# include "MobSystem.hh"
# include "BonusSystem.hh"
# include "BossSystem.hh"
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
      delete system["boss"];
    }
  }

  Game() : refresh(Enum::REFRESH_TIME), is_playing(false),
	   lvl_name("level1") {
    system["shoot"] = new ShootSystem;
    system["monsters"] = new MobSystem; 
    system["bonuses"] = new BonusSystem;
    system["boss"] = new BossSystem;
  }

  uint64_t	getId() {
    return (ids++);
  }

    std::unordered_map<std::string, ASystem *>  system;
    std::string                                 name;
    std::list<UserManager<SCK> *>               players;
    uint64_t                                 refresh;
    bool                                        is_playing;
    float					time;
    std::string					lvl_name;
    std::unordered_map<std::string, Entity>	entities;
    Entity					*level;
    std::unordered_map<std::string, std::string>  content_system;
    uint64_t		ids = 0;
    uint64_t		shoot_player_ids = Enum::MAX_ID;
    uint64_t		monster_ids = 2 * Enum::MAX_ID;
    uint64_t		bonus_ids = 3 * static_cast<uint64_t>(Enum::MAX_ID);
    uint64_t		shoot_mob_ids = 4 * static_cast<uint64_t>(Enum::MAX_ID);
    uint64_t	        boss_ids = 5 * static_cast<uint64_t>(Enum::MAX_ID);
};

template <typename SCK>
class GameManager {
public:
    static  GameManager             &instance();
    static  uint64_t             getTime();
    static  uint64_t             getTimeInSecond();
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
    IServerSocket<SCK>		    *getUDPSocket();
    bool			    isAllDead(Game<SCK> *) const;
    void			    synchronisation(Game<SCK> *);
    bool			    bossIsDead(Game<SCK> *);
private:
    bool                            update(Game<SCK> *game, uint64_t);
    void                            updatePositions(Game<SCK> *, uint64_t);
    bool			    updateTime(Game<SCK> *);
    bool		            updateObjSighting(Game<SCK> *game,
						      uint64_t time,
						      const std::string &);
    bool				updateBoss(Game<SCK> *);
    bool			    checkEntities(Game<SCK> *, 
						  std::pair<std::string, Entity&>,
						  int,
						  uint64_t,
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

