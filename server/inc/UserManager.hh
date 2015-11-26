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
    void					setPing(bool p = true);
    bool					getPing() const;
    std::string				getFriendName();
    std::string				getPacketData() const;
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
    bool					deleteUserWith(const std::string &s,
                                           std::fstream &file,
                                           std::string &stock);
    const std::string       &getGameroomName() const;

    Enum::TCPServerAnswers		verifyUser();
    Enum::TCPServerAnswers	    newUser();
    Enum::TCPServerAnswers      startUpload();
    Enum::TCPServerAnswers      uploadImage();
    Enum::TCPServerAnswers      endOfUploading();
    Enum::TCPServerAnswers      deleteImage();
    Enum::TCPServerAnswers      retrieveImage();

    Enum::TCPServerAnswers      joinRandomRoom();
    Enum::TCPServerAnswers      joinNamedRoom();
    Enum::TCPServerAnswers      createGameRoom();
    Enum::TCPServerAnswers      leaveRoom();
    Enum::TCPServerAnswers      ready();
    Enum::TCPServerAnswers      notReady();


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
    std::string                             gameroom;

    bool					hasBadFormat(std::string *) const;
    bool					alreadyExist(std::string *);

};


#endif // !USERMANAGER_H_
