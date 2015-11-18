#include <algorithm>
#include "UserManager.h"

template<typename T>
const std::string UserManager<T>::database_dir = "./server/.database/.%_data";

template<typename T>
UserManager<T>::UserManager(IServerSocket<T> *sck) :
  sock(sck), call_query(Enum::NONE_QUERY) {
  status = Enum::ONLINE;
  clearData();
}

template<typename T>
UserManager<T>::~UserManager() {
  stream.close();
  delete this->sock;
}

template<typename T>
Enum::UserStatus	UserManager<T>::getStatus() const { return (status); }

template<typename T>
bool			UserManager<T>::isLogged() const { return (stream.is_open()); }

template<typename T>
void	UserManager<T>::writeStruct(const ::ComData &comdata) {
  packet.stockOnBuff(comdata);
}

template<typename T>
Enum::ServerAnswers	UserManager<T>::changeStatus() {
  int			i;
  std::string		s;
  std::istringstream	is;

  if (!stream.is_open())
    return (Enum::ENOT_LOGGED);
  is.str(tmp_packet.data);
  is >> i;
  if (i >= 0 && i <= 4) {
    stream.clear();
    stream.seekg(0, stream.beg);
    std::getline(stream, s, '|');
    stream.seekg(-2, stream.cur);
    stream << i << std::flush;
    status = static_cast<Enum::UserStatus>(i);
  }
  return (Enum::OK);
}

template<typename T>
void	UserManager<T>::readFromMe() {
  packet.getPacket<IServerSocket<T> *>(sock);
}

template<typename T>
void	UserManager<T>::writeMsg(const std::string &s) {
  packet.stockOnBuff(s);
}

template<typename T>
bool	UserManager<T>::writeOnMe()
{
  packet.serialize();
  return (packet.sendPacket<IServerSocket<T> *>(sock));
}

template<typename T>
bool	UserManager<T>::sendStructEmpty() const { return (packet.sendPackEmpty()); }

template<typename T>
IServerSocket<T>	*UserManager<T>::getServerSocket() { return (this->sock); }

template<typename T>
bool	UserManager<T>::IsFilled() const {
  return (packet.isFilled());
}

template<typename T>
void	UserManager<T>::clearData() {
  packet.clearAll();
}

template<typename T>
void	UserManager<T>::fillPacketStruct() { tmp_packet = packet.retrievePacket(); }

template<typename T>
bool	UserManager<T>::emptyData() const {
  return (packet.packetEmpty());
}

template<typename T>
Enum::ClientQueries	UserManager<T>::numQuery() const {
  return (static_cast<Enum::ClientQueries>(tmp_packet.com.query));
}

template<typename T>
std::string	UserManager<T>::getPacketData() const { return (std::string(tmp_packet.data)); }

template<typename T>
std::string	UserManager<T>::getIP() const { return (this->sock->getIP()); }

template<typename T>
Enum::ServerAnswers	UserManager<T>::verifyUser() {
  std::string s(tmp_packet.data);
  std::string tmp;
  std::string tok;
  std::istringstream is(s);
  std::string	pass;
  std::string	tmp_log;

  std::getline(is, tok, ':');
  if (stream.is_open())
    return (Enum::EALREADY_LOGGED);
  stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
							   1, tok),
	      std::fstream::in | std::fstream::out);
  stream.seekg(0, stream.beg);
  if (!stream.is_open())
    return (Enum::EUSERPASS);
  std::getline(stream, tmp, '|');
  std::getline(is, pass, ':');
  tmp_log = tok + ":" + pass;
  if (!tmp.compare(0, tmp_log.size(), tmp_log)
      && (tmp.size() - 2 == tmp_log.size())) {
    name = tok;
    status = Enum::ONLINE;
    stream.seekg(-2, stream.cur);
    stream << "0" << std::flush;
    stream.clear();
    stream.seekg(0, stream.end);
    return (Enum::LOGGED_IN);
  }
  stream.close();
  return (Enum::EUSERPASS);
}

template<typename T>
std::string	UserManager<T>::getFriendName() {
  std::istringstream	is;
  std::string	s;

  if (!stream.is_open())
    return ("");
  std::getline(stream, s, '|');
  return (s);
}

template<typename T>
Enum::ServerAnswers	UserManager<T>::newUser() {
  std::string s(tmp_packet.data);
  std::istringstream is(s);
  std::string tok[2];

  if (stream.is_open())
    return (Enum::EALREADY_LOGGED); // TODO, not really this error !
  std::getline(is, tok[0], ':');
  std::getline(is, tok[1], ':');
  if (alreadyExist(tok))
    {
      stream.close();
      return (Enum::EUSER_EXIST);
    }
  if (hasBadFormat(tok))
    return (Enum::EREGISTER_FORMAT);
  stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
							   1, tok[0]).c_str(),
	      std::fstream::in |
	      std::fstream::out |
	      std::fstream::app);
  if (!stream.is_open())
    return (Enum::EUSER_EXIST);
  stream.clear();
  stream << tok[0] << ":" << tok[1] <<
    ":" << static_cast<int>(Enum::ONLINE) << "|" << std::flush;
  name = tok[0];
  stream.close();
  return (Enum::OK);
}

template<typename T>
bool	UserManager<T>::hasBadFormat(std::string *info) const {
  for (int i = 0; i < 2; ++i) {
    auto it = std::find_if_not(info[i].begin(), info[i].end(), [](char c) {
        return ((c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9') || 
		(c == '-' || c == '_'));
      });
    if (it != info[i].end())
      return (true);
  }
  return (((info[0].size() < Enum::REGISTER_MIN) || (info[0].size() > Enum::REGISTER_MAX)) ||
	  ((info[1].size() < Enum::REGISTER_MIN) || (info[1].size() > Enum::REGISTER_MAX)));
}

template<typename T>
bool			UserManager<T>::alreadyExist(std::string *info) {
  stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
							   1, info[0]),
	      std::fstream::in | std::fstream::out);
  return (stream.is_open());
}

template<typename T>
void		UserManager<T>::callWith(const std::string &s, Enum::ClientQueries q) {
  call_with = s;
  call_query = q;
}

template<typename T>
const std::string	&UserManager<T>::getCallUser() const { return (call_with); }

template<typename T>
Enum::ClientQueries	UserManager<T>::getCallQuery() const { return (call_query); }

template<typename T>
void	UserManager<T>::fromFirstUser() {
  std::string s;

  if (!stream.is_open())
    return ;
  stream.clear();
  stream.seekg(0, stream.beg);
  stream.clear();
  std::getline(stream, s, '|');
}

template<typename T>
void	UserManager<T>::sendPing() { writeStruct({0, Enum::PING}); }

template<typename T>
void	UserManager<T>::setPing(bool p) { ping = p; }

template<typename T>
bool	UserManager<T>::getPing() const { return (ping); }

template<typename T>
void	UserManager<T>::disconnect() {
  if (!stream.is_open())
    return ;
  stream.close();
  name.clear();
}

template<typename T>
bool	UserManager<T>::deleteUserWith(const std::string &s,
				       std::fstream &file,
				       std::string &stock) {
  std::string	        tmp;
  bool			found = false;

  stock.clear();
  file.clear();
  file.seekg(0, file.beg);
  std::getline(file, tmp, '|');
  stock = tmp + "|";
  do {
    file.clear();
    std::getline(file, tmp, '|');
    if (tmp == s)
      found = true;
    else {
      if (!tmp.empty())
	stock += tmp + "|";
    }
  } while (!file.eof());
  file.close();
  return (found);
}

template<typename T>
Enum::ServerAnswers	UserManager<T>::deleteUserAsFriend() {
  std::string		s(tmp_packet.data);
  std::string	        stock;
  bool			found = false;
  std::fstream		f;

  if (!stream.is_open())
    return (Enum::ENOT_LOGGED);  
  if (!contactExist() || !deleteUserWith(tmp_packet.data,
					 stream, stock))
    return (Enum::ENO_EXIST);
  stream.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
								1, name),
	      std::fstream::in | std::fstream::out | std::fstream::trunc);
  if (!stream.is_open())
    return (Enum::ENOT_LOGGED);
  stream << stock << std::flush;
  stream.clear();
  stream.seekg(0, stream.end);
  f.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
							   1, s),
	 std::fstream::in);
  if (!f.is_open())
    return (Enum::ENO_EXIST);
  deleteUserWith(name, f, stock);
  f.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
								1, s),
	 std::fstream::out | std::fstream::trunc);
  if (!f.is_open())
    return (Enum::ENO_EXIST);
  f << stock << std::flush;
  f.close();
  return (Enum::OK);
}

template<typename T>
Enum::ServerAnswers	UserManager<T>::addUserAsFriend() {
  std::string		s(tmp_packet.data);

  if (!stream.is_open())
    return (Enum::ENOT_LOGGED);
  if (friendAlreadyExist())
    return (Enum::EALREADY_ADDED);
  if (!contactExist())
    return (Enum::ENO_EXIST);
  stream.clear();
  stream << s  << "|" << std::flush;
  return (Enum::OK);
}

template<typename T>
const std::string	&UserManager<T>::getName() const { return (name); }

template<typename T>
bool	UserManager<T>::friendAlreadyExist() {
  std::string		login(tmp_packet.data);
  std::fstream		f;

  if (name == login)
    return (true);
  stream.clear();
  stream.seekg(0, stream.beg);
  if (friendMatch(login, stream)) {
      stream.seekg(0, stream.end);
      return (true);
  }
  f.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
							   1, login),
	 std::fstream::in);
  if (!f.is_open())
    return (false);
  f.close();
  f.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
							   1, login),
	 std::fstream::in | std::fstream::out | std::fstream::app);
  f << name << "|" << std::flush;
  f.close();
  return (false);
}

template<typename T>
bool	UserManager<T>::contactExist() {
  std::fstream	f;

  f.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
						      1,
						      std::string(tmp_packet.data)),
	 std::fstream::in |
	 std::fstream::out);
  if (!f.is_open())
    return (false);
  f.close();
  return (true);
}

template <typename T>
void	UserManager<T>::checkCall() {
  for (auto it = call_list.begin(); it != call_list.end();) {
    if (time(0) - it->time >= Enum::CALL_TIME) {
      refuseCall(it);
      call_list.erase(it++);
    }
    else
      ++it;
  }
}

template <typename T>
void	UserManager<T>::addOnCallList(const std::string &n,
				      UserManager<T> *cl,
				      Enum::ClientQueries q) {
  Call	call;

  for (auto it = call_list.begin(); it != call_list.end(); ++it)
    if (it->name == n)
      return ;
  call.time = time(0);
  call.audio_type = q;
  call.name = n;
  call.cl = cl;
  call_list.push_back(call);
}

template <typename T>
bool	UserManager<T>::deleteCallByName(const std::string &n) {
  for (auto it = call_list.begin(); it != call_list.end(); ++it) {
    if (it->name == n) {
      call_list.erase(it);
      return (true);
    }
  }
  return (false);
}

template<typename T>
bool	UserManager<T>::friendMatch(const std::string &login,
			       std::fstream &f) {
  std::istringstream	is("");
  std::string		s = "";

  stream.clear();
  if (login.empty())
    return (false);
  do {
    f.clear();
    std::getline(f, s, '|');
    if (s == login)
      return (true);
    is.str("");
  } while (!f.eof());
  return (false);
}

template<typename T>
Enum::ServerAnswers	UserManager<T>::userExist() {
  std::fstream		f;
  std::string		name;
  std::istringstream	is(tmp_packet.data);

  if (!stream.is_open())
    return (Enum::ENOT_LOGGED);
  std::getline(is, name, ':');
  f.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
						      1, name),
	 std::fstream::in);
  if (f.is_open()) {
    f.close();
    return (Enum::OK);
  }
  return (Enum::ENO_EXIST);
}

template<typename T>
void	UserManager<T>::stockMessage(const std::string &u_name) {
  std::fstream f;

  f.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
						      1,
						      u_name + "_msg"),
	 std::fstream::out | std::fstream::app);
  if (!f.is_open())
    return ;
  std::replace_if(tmp_packet.data.begin(),
		  tmp_packet.data.end(),
		  [](char c) { return (c == '|'); },
		  '#');
  f << tmp_packet.data << "|" << std::flush;
  f.close();
}

template<typename T>
void	UserManager<T>::retrieveMessage() {
  std::fstream f;
  std::string  s = " ";

  f.open(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
						      1,
						      name + "_msg"),
	 std::fstream::in);
  if (!f.is_open())
    return ;
  while (!f.eof()) {
    std::getline(f, s, '|');
    if (!s.empty()) {
      writeStruct({static_cast<uint16_t>(s.size()),
	    Enum::INCOMING_TEXT});
      writeMsg(s);
    }
  }
  f.close();
#if (defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64))
  DeleteFile(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
							  1, name + "_msg").c_str());
#else
  remove(std::string(UserManager<T>::database_dir).replace(Enum::PATH_LENGTH,
						      1, name + "_msg").c_str());
#endif
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined (_WIN64)
template class UserManager<SOCKET>;
#else
template class UserManager<int>;
#endif
