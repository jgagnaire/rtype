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
# include "Enum.hh"
# include "Packet.hh"
# include "IServerSocket.hh"

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
#  include "WinServerSocket.hh"
# else
#  include "UnixServerSocket.hh"
# endif

struct Position {
    std::size_t x;
    std::size_t y;
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

    bool					IsFilled() const;
    void					clearData();
    void					readFromMe();

    bool		        	isLogged() const;
    bool					emptyData() const;
    Enum::ClientQueries		numQuery() const;
    Enum::ClientQueries     numUDPQuery() const;
    void					setPing(bool p = true);
    bool					getPing() const;
    std::string				getPacketData() const;
    std::string				getUDPPacketData() const;
    const std::string		&getName() const;
    std::string				getIP() const;
    bool					sendStructEmpty() const;
    void					writeStruct(const ::TCPDataHeader &);
    void					writeMsg(const std::string &);
    bool 					writeOnMe();
    IServerSocket<T>		*getServerSocket();
    void					disconnect();
    void		        	fillPacketStruct();
    void					sendPing();
    const std::string       &getGameroomName() const;
    bool                    isReady() const;
    Enum::UserStatus        getStatus() const;
    void                    inGame();
    void                    setUdpPacketStruct(const Packet<UDPDataHeader>::PacketStruct &);
    void                    clearGameData();
    uint64_t                getUDPPacketId();

    Enum::ServerAnswers		verifyUser();
    Enum::ServerAnswers	    newUser();
    Enum::ServerAnswers     joinRandomRoom();
    Enum::ServerAnswers     joinNamedRoom();
    Enum::ServerAnswers     createGameRoom();
    Enum::ServerAnswers     leaveRoom();
    Enum::ServerAnswers     ready();
    Enum::ServerAnswers     notReady();
    Enum::ServerAnswers     getRoomList();
    Enum::ServerAnswers     quitGame();
    Enum::ServerAnswers     currentPosition();
    Enum::ServerAnswers     keyPressed();
    Enum::ServerAnswers     audioPacket();
    Enum::ServerAnswers     takeForce();

private:
    IServerSocket<T>						*sock;
    Enum::UserStatus                        status;
    std::string								name;
    std::fstream							stream;
    std::fstream                            image_stream;
    static const std::string				database_dir;
    bool									ping;
    Packet<TCPDataHeader>					packet;
    Packet<TCPDataHeader>::PacketStruct		tmp_packet;
    Packet<UDPDataHeader>::PacketStruct		udp_packet;
    std::string                             gameroom;


    uint64_t                                udp_packet_id;
    bool                                    is_ready;
    std::size_t                             keypressed;
    bool                                    has_force;
    Position                                tmp_pos;
    Position                                real_pos;

    bool					hasBadFormat(std::string *) const;
    bool					alreadyExist(std::string *);
    std::string             generateRoomName();

};


#endif // !USERMANAGER_H_
