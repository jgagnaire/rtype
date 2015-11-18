#include "AccountController.h"

template<typename T>
AccountController<T>::AccountController(std::list<UserManager<T> *> &cli)
  : AController<T>(cli) {
  this->proto_map[Enum::ACCOUNT_LOGIN] = &AccountController<T>::accountLoginFct;
  this->proto_map[Enum::ACCOUNT_REGISTER] = &AccountController<T>::accountRegisterFct;
  this->proto_map[Enum::DISCONNECTION] = &AccountController<T>::disconnectionFct;
  this->proto_map[Enum::CONTACT_ADD] = &AccountController<T>::contactAddFct;
  this->proto_map[Enum::CONTACT_DELETION] = &AccountController<T>::contactDeleteFct;
  this->proto_map[Enum::STATUS] = &AccountController<T>::changeStatus;
  this->proto_map[Enum::PONG] = &AccountController<T>::pong;
}

template<typename T>
int	AccountController<T>::newData(UserManager<T> *cli) {
  cli->checkCall();
  cli->fillPacketStruct();
  if (proto_map.find(cli->numQuery()) == proto_map.end())
    return (-1);
  return (static_cast<int>((this->*proto_map[cli->numQuery()])(cli)));
}

template<typename T>
bool    AccountController<T>::changeStatus(UserManager<T> *cl) const {
  Enum::ServerAnswers sa = cl->changeStatus();
  std::string s = "";
  std::string cl_str = "";

  cl->writeStruct({0, sa});
  if (sa == Enum::OK) {
    cl->fromFirstUser();
    cl_str += cl->getName() + ":" + static_cast<char>(cl->getStatus() + 48);
    do {
      s = cl->getFriendName();
      UserManager<T> *tmp_cl = this->findUserByName(s);
      if (tmp_cl) {
	tmp_cl->writeStruct({static_cast<uint16_t>(cl_str.size()),
	      Enum::CHANGE_STATUS});
	tmp_cl->writeMsg(cl_str);
      }
    } while (!s.empty());
  }
  return (true);
}

template<typename T>
bool    AccountController<T>::pong(UserManager<T> *cl) const {
  cl->setPing(true);
  return (true);
}

template<typename T>
int    AccountController<T>::newConnection(UserManager<T> *cl) const {
  cl->sendPing();
  return (1);
}

template<typename T>
bool    AccountController<T>::accountLoginFct(UserManager<T> *cl) const {
  Enum::ServerAnswers sa = cl->verifyUser();
  std::string s = " ";
  std::string	cl_str;
  std::string	tmp_cl_str;

  cl->writeStruct({0, sa});
  if (sa == Enum::LOGGED_IN) {
    cl->fromFirstUser();
    while (!s.empty()) {
      s = cl->getFriendName();
      UserManager<T> *tmp_cl = this->findUserByName(s);
      if (!s.empty()) {
	if (tmp_cl) {
	  cl_str = cl->getName() + ":" + static_cast<char>(cl->getStatus() + 48);
	  tmp_cl_str = tmp_cl->getName() + ":" + static_cast<char>(tmp_cl->getStatus() + 48);
	  tmp_cl->writeStruct({static_cast<uint16_t>(cl_str.size()),
		Enum::CHANGE_STATUS});
	  tmp_cl->writeMsg(cl_str);
	  cl->writeStruct({static_cast<uint16_t>(tmp_cl_str.size()),
		Enum::CONTACT_LIST});
	  cl->writeMsg(tmp_cl_str);
	}
	else {
	  tmp_cl_str = s + ":1";
	  cl->writeStruct({static_cast<uint16_t>(tmp_cl_str.size()),
		Enum::CONTACT_LIST});
	  cl->writeMsg(tmp_cl_str);
	}
      }
    }
  }
  cl->retrieveMessage();
  return (true);
}

template<typename T>
bool    AccountController<T>::accountRegisterFct(UserManager<T> *cl) const {
  cl->writeStruct({0, cl->newUser()});
  return (true);
}

template<typename T>
bool	AccountController<T>::disconnectionFct(UserManager<T> *cl) const {
  std::string	s = " ";
  std::string	cl_str;

  if (!cl->isLogged())
    return (false);
  cl->fromFirstUser();
  while (!s.empty()) {
    s = cl->getFriendName();
    UserManager<T> *tmp_cl = this->findUserByName(s);
    if (tmp_cl) {
      cl_str = cl->getName() + ":1";
      tmp_cl->writeStruct({static_cast<uint16_t>(cl_str.size()),
	    Enum::CHANGE_STATUS});
      tmp_cl->writeMsg(cl_str);
    }
  }
  UserManager<T> * tmp_cl = this->findUserByName(cl->getCallUser());
  if (tmp_cl) {
    if (static_cast<int>(tmp_cl->getCallQuery()) &
	static_cast<int>(Enum::AUDIO_CALL_INIT)) {
      tmp_cl->writeStruct({ static_cast<uint16_t>(cl->getName().size()),
	    Enum::USER_HANG_UP });
      tmp_cl->writeMsg(cl->getName());
    }
    if (static_cast<int>(tmp_cl->getCallQuery()) &
	static_cast<int>(Enum::VIDEO_CALL_INIT)) {
      tmp_cl->writeStruct({ static_cast<uint16_t>(cl->getName().size()),
	    Enum::USER_VIDEO_HANG_UP });
      tmp_cl->writeMsg(cl->getName());
    }
    tmp_cl->callWith("", Enum::NONE_QUERY);
  }
  for (auto it = cl->call_list.begin(); it != cl->call_list.end();) {
    if (this->findUserByName(it->name))
      cl->refuseCall(it);
    cl->call_list.erase(it++);
    ++it;
  }
  cl->disconnect();
  return (true);
}

template<typename T>
int    AccountController<T>::closeConnection(UserManager<T> *cl) const {
  disconnectionFct(cl);
  return (0);
}

template<typename T>
bool	AccountController<T>::contactAddFct(UserManager<T> *cl) const {
  Enum::ServerAnswers sa = cl->addUserAsFriend();
  std::string cl_str;
  std::string tmp_cl_str;

  cl->writeStruct({ 0, sa });
  if (sa == Enum::OK) {
    UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());
    if (tmp_cl) {
	  cl_str = cl->getName() + ":" + static_cast<char>(cl->getStatus() + 48);
	  tmp_cl_str = tmp_cl->getName() + ":" + static_cast<char>(tmp_cl->getStatus() + 48);
	  tmp_cl->writeStruct({static_cast<uint16_t>(cl->getName().size()),
		Enum::USER_ADDED});
	  tmp_cl->writeMsg(cl->getName());
	  tmp_cl->writeStruct({ static_cast<uint16_t>(cl_str.size()),
		Enum::CHANGE_STATUS });
	  tmp_cl->writeMsg(cl_str);
	  cl->writeStruct({ static_cast<uint16_t>(tmp_cl_str.size()),
		Enum::CHANGE_STATUS });
	  cl->writeMsg(tmp_cl_str);
    }
  }
  return (true);
}

template<typename T>
bool	AccountController<T>::contactDeleteFct(UserManager<T> *cl) const {
  UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());
  cl->writeStruct({ 0, cl->deleteUserAsFriend() });
  if (tmp_cl) {
    tmp_cl->writeStruct({static_cast<uint16_t>(cl->getName().size()),
					       Enum::USER_DELETED});
    tmp_cl->writeMsg(cl->getName());
  }
  return (true);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
template class AccountController<SOCKET>;
#else
template class AccountController<int>;
#endif
