#ifndef USERMANAGER_H_
# define USERMANAGER_H_

# include <string>
# include <cstring>
# include <cstdint>
# include <algorithm>
# include <sstream>
# include <fstream>
# include <queue>
# include <iostream>
# include <time.h>
# include <list>
# include <mutex>
# include "Enum.hh"
# include "Packet.hh"
# include "Entity.hh"
# include "IServerSocket.hh"

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
#   include "WinServerSocket.hh"
#   include "WinMutex.hh"
# else
#   include "UnixServerSocket.hh"
#   include "UnixMutex.hh"
# endif

struct Position {
    float x;
    float y;
    void        clear() {
        x = 0;
        y = 0;
    }
};

template <typename T>
class UserManager
{
public:
    UserManager(IServerSocket<T> *sck);
    ~UserManager();

    static std::mutex       user_mutex;
    bool					IsFilled() const;
    void					clearData();
    void					readFromMe();

    bool		    isLogged() const;
    bool		    emptyData() const;
    Enum::ClientQueries	    numQuery() const;
    Enum::ClientQueries     numUDPQuery() const;
    void		    setPing(bool p = true);
    bool		    getPing() const;
    std::string		    getPacketData() const;
    std::string		    getUDPPacketData() const;
    const std::string	    &getName() const;
    std::string		    getIP() const;
    bool		    sendStructEmpty() const;
    void		    writeStruct(const ::TCPDataHeader &);
    void		    writeMsg(const std::string &);
    bool		    writeOnMe();
    IServerSocket<T>        *getServerSocket();
    void		    disconnect();
    void		    fillPacketStruct();
    void		    sendPing();
    const std::string       &getGameroomName() const;
    bool                    isReady() const;
    Enum::UserStatus        getStatus() const;
    void                    inGame();
    void                    setUdpPacketStruct(const Packet<UDPDataHeader>::PacketStruct &);
    void                    setUdpBinaryPacketStruct(UDPData &);
    const UDPData	    &getUdpBinaryPacketStruct() const;
    void                    clearGameData();
    uint64_t                getUDPPacketId();
    bool                    isFiring() const;
    bool                    isDead() const;
    void                    isTouched(int damage = Enum::MAX_DAMAGE);
    void                    changePosition(std::size_t);
    const Position	    &getPosition() const;
    const std::size_t	    &getKeypressed() const;
    void		    setId(std::size_t);
    std::size_t		    getId();
    void	            getBonus(Entity *);
    bool		    updateBonus(std::size_t);
    bool		    isRespawning() const;
    const std::size_t       *getLatency();

    Enum::ServerAnswers	    verifyUser();
    Enum::ServerAnswers	    newUser();
    Enum::ServerAnswers     joinNamedRoom();
    Enum::ServerAnswers     createGameRoom();
    Enum::ServerAnswers     leaveRoom();
    Enum::ServerAnswers     ready();
    Enum::ServerAnswers     notReady();
    Enum::ServerAnswers     getRoomList();
    Enum::ServerAnswers     quitGame();
    Enum::ServerAnswers     currentPosition();
    Enum::ServerAnswers     keyPressed();
    Enum::ServerAnswers     takeForce();
    Enum::ServerAnswers	    setLatency();
    std::mutex              destroy_client_mutex;

private:
  IServerSocket<T>			  *sock;
  Enum::UserStatus                        status;
  std::string				  name;
  std::fstream				  stream;
  std::fstream                            image_stream;
  static const std::string		  database_dir;
  bool					  ping;
  Packet<TCPDataHeader>			  packet;
  Packet<TCPDataHeader>::PacketStruct	  tmp_packet;
  Packet<UDPDataHeader>::PacketStruct	  udp_packet;
  UDPData				  udp_binary_packet;
  std::string                             gameroom;
  std::mutex                              game_mutex;

  uint64_t                                udp_packet_id;
  bool                                    is_ready;
  std::size_t                             keypressed;
  std::size_t				  last_duration;
  bool                                    has_force;
  bool                                    fire;
  bool					  switch_weapon;
  Position                                position;
  bool					  is_dead;
  char					  life;
  int64_t				  id;
  int64_t				  force = 0;
  int64_t				  protection = 0;
  int64_t				  perfect_shield = 0;
  int64_t				  respawn = 0;
  std::size_t				  latency[4];

  bool					hasBadFormat(std::string *) const;
  bool					alreadyExist(std::string *);
  std::string				generateRoomName();

};


#endif // !USERMANAGER_H_
