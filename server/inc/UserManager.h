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
# include "Enum.h"
# include "Packet.h"
# include "IServerSocket.h"
# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
#  include "WinServerSocket.h"
# else
#  include "UnixServerSocket.h"
# endif

template <typename T>
class UserManager;

template <typename T>
class UserManager
{
 public:
  UserManager(IServerSocket<T> *sck);
  ~UserManager();

  struct	Call {
    time_t			time;
    Enum::ClientQueries		audio_type;
    std::string			name;
    UserManager			*cl;
  };

  bool	IsFilled() const;
  void	clearData();
  void	readFromMe();

  bool		        isLogged() const;
  bool			emptyData() const;
  void			retrieveMessage();
  Enum::UserStatus      getStatus() const;
  Enum::ClientQueries	numQuery() const;
  void			setPing(bool p = true);
  bool			getPing() const;
  std::string		getFriendName();
  std::string		getPacketData() const;
  void			fromFirstUser();
  Enum::ServerAnswers	deleteUserAsFriend();
  Enum::ServerAnswers	verifyUser();
  Enum::ServerAnswers	newUser();
  Enum::ServerAnswers	addUserAsFriend();
  Enum::ServerAnswers   userExist();
  Enum::ServerAnswers   changeStatus();
  const std::string	&getName() const;
  std::string		getIP() const;
  bool			sendStructEmpty() const;
  void			writeStruct(const ::ComData &);
  void			writeMsg(const std::string &);
  bool writeOnMe();
  IServerSocket<T>	*getServerSocket();
  void			disconnect();
  void			stockMessage(const std::string &);
  void		        fillPacketStruct();
  void			sendPing();
  void			checkCall();
  void			addOnCallList(const std::string &,
				      UserManager *cl,
				      Enum::ClientQueries);
  bool			deleteCallByName(const std::string &);
  void			callWith(const std::string &, Enum::ClientQueries);
  const std::string	&getCallUser() const;
  Enum::ClientQueries	getCallQuery() const;
  bool			deleteUserWith(const std::string &s,
				       std::fstream &file,
				       std::string &stock);
  std::list<Call>	call_list;

  void			refuseCall(typename std::list<Call>::iterator &it) const {
    if (it->cl) {
      if (it->audio_type == Enum::AUDIO_CALL_INIT)
	it->cl->writeStruct({static_cast<uint16_t>(it->name.size()),
	      Enum::REJECT_AUDIO});
      else
	it->cl->writeStruct({static_cast<uint16_t>(it->name.size()),
	      Enum::REJECT_VIDEO});
      it->cl->writeMsg(name);
    }
  }

private:
  IServerSocket<T>		*sock;
  std::string			name;
  Enum::UserStatus		status;
  std::fstream			stream;
  static const std::string	database_dir;
  bool				ping;
  Packet			packet;
  PacketStruct			tmp_packet;
  std::string			call_with;
  Enum::ClientQueries		call_query;

  bool			hasBadFormat(std::string *) const;
  bool			alreadyExist(std::string *);
  bool			friendAlreadyExist();
  bool		        friendMatch(const std::string &, std::fstream &);
  bool		        contactExist();

};


#endif // !USERMANAGER_H_
