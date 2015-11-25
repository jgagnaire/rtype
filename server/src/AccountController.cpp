#include "AccountController.hh"

template<typename T>
AccountController<T>::AccountController(std::list<UserManager<T> *> *cli)
  : AController<T>(*cli) {}

template<typename T>
int	AccountController<T>::newData(UserManager<T> *cli) {
  cli->fillPacketStruct();
  switch (cli->numQuery()) {
  case Enum::ACCOUNT_LOGIN:
    return (static_cast<int>(accountLoginFct(cli)));
  case Enum::ACCOUNT_REGISTER:
    return (static_cast<int>(accountRegisterFct(cli)));
  case Enum::DISCONNECTION:
    return (static_cast<int>(disconnectionFct(cli)));
  case Enum::STATUS:
    return (static_cast<int>(changeStatus(cli)));
  case Enum::PONG:
    return (static_cast<int>(pong(cli)));
  default:
    return (-1);
  }
  return (-1);
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
  cl->disconnect();
  return (true);
}

template<typename T>
int    AccountController<T>::closeConnection(UserManager<T> *cl) const {
  disconnectionFct(cl);
  return (0);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
template class AccountController<SOCKET>;
#else
template class AccountController<int>;
#endif
