#include "ConversationController.h"

template<typename T>
ConversationController<T>::ConversationController(std::list<UserManager<T> *> &cli)
  : AController<T>(cli) {
  this->proto_map[Enum::SEND_TEXT] = &ConversationController<T>::sendTextFct;
  this->proto_map[Enum::AUDIO_CALL_INIT] = &ConversationController<T>::audioCallInitFct;
  this->proto_map[Enum::VIDEO_CALL_INIT] = &ConversationController<T>::videoCallInitFct;
  this->proto_map[Enum::AUDIO_CALL_ACCEPT] = &ConversationController<T>::audioCallAcceptFct;
  this->proto_map[Enum::VIDEO_CALL_ACCEPT] = &ConversationController<T>::videoCallAcceptFct;
  this->proto_map[Enum::AUDIO_CALL_REJECT] = &ConversationController<T>::audioCallRejectFct;
  this->proto_map[Enum::VIDEO_CALL_REJECT] = &ConversationController<T>::videoCallRejectFct;
  this->proto_map[Enum::HANG_UP] = &ConversationController<T>::hangUpFct;
  this->proto_map[Enum::HANG_UP_VIDEO] = &ConversationController<T>::hangUpVideoFct;
}

template<typename T>
void ConversationController<T>::hangUp(UserManager<T> *cl,
				       Enum::ClientQueries cq,
				       Enum::ServerNotification type) const {
  UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());

  if (!cl->isLogged())
    {
      cl->writeStruct({ 0, Enum::ENOT_LOGGED });
      return ;
    }
  cl->writeStruct({ 0, Enum::OK });
  if (tmp_cl) {
    if ((static_cast<int>(tmp_cl->getCallQuery()) &
	  static_cast<int>(cq))) {
      tmp_cl->writeStruct({ static_cast<uint16_t>(cl->getName().size()),
	    type });
      tmp_cl->callWith(cl->getName(),
		       static_cast<Enum::ClientQueries>(static_cast<int>(tmp_cl->getCallQuery()) ^
							static_cast<int>(cq)));
      tmp_cl->writeMsg(cl->getName());
      cl->callWith(tmp_cl->getName(),
		   static_cast<Enum::ClientQueries>(static_cast<int>(cl->getCallQuery()) ^
						    static_cast<int>(cq)));
    }
    if (tmp_cl->getCallQuery() == Enum::NONE_QUERY) {
      tmp_cl->callWith("", Enum::NONE_QUERY);
      cl->callWith("", Enum::NONE_QUERY);
    }
  }
}

template<typename T>
bool ConversationController<T>::hangUpVideoFct(UserManager<T> *cl) const {
  hangUp(cl, Enum::VIDEO_CALL_INIT, Enum::USER_VIDEO_HANG_UP);
  return (true);
}

template<typename T>
bool ConversationController<T>::hangUpFct(UserManager<T> *cl) const {
  hangUp(cl, Enum::AUDIO_CALL_INIT, Enum::USER_HANG_UP);
  return (true);
}

template<typename T>
bool ConversationController<T>::sendTextFct(UserManager<T> *cl) const {
  Enum::ServerAnswers sa = cl->userExist();
  std::string		s;
  std::istringstream	is(cl->getPacketData());
  std::string		text;

  std::getline(is, s, ':');
  std::getline(is, text, '\0');
  text = cl->getName() + ":" + text;
  cl->writeStruct({ 0, sa });
  if (sa == Enum::OK) {
    UserManager<T> * tmp_cl = this->findUserByName(s);
    if (tmp_cl) {
      tmp_cl->writeStruct({static_cast<uint16_t>(text.size()),
	    Enum::INCOMING_TEXT});
      tmp_cl->writeMsg(text);
    }
    else
      cl->stockMessage(s);
  }
  return (true);
}

template<typename T>
bool ConversationController<T>::audioCallInitFct(UserManager<T> *cl) const {
  UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());

  if (!cl->isLogged())
    {
      cl->writeStruct({0, Enum::ENOT_LOGGED});
      return (true);
    }
  if (tmp_cl) {
    if (!(static_cast<int>(tmp_cl->getCallQuery()) ^
	  static_cast<int>(Enum::AUDIO_CALL_INIT))) {
      cl->writeStruct({ static_cast<uint16_t>(tmp_cl->getName().size()),
	    Enum::USER_HANG_UP });
      cl->writeMsg(tmp_cl->getName());
      return (true);
    }
    cl->writeStruct({0, Enum::OK});
    tmp_cl->writeStruct({static_cast<uint16_t>(cl->getName().size()),
	  Enum::INCOMING_AUDIO});
    tmp_cl->writeMsg(cl->getName());
    tmp_cl->addOnCallList(cl->getName(), cl, Enum::AUDIO_CALL_INIT);
  }
  else
    cl->writeStruct({0, Enum::ENO_EXIST});
  return (true);
}

template<typename T>
int     ConversationController<T>::newData(UserManager<T> *cli) {
  for (auto it = cli->call_list.begin(); it != cli->call_list.end();) {
    if (!this->findUserByName(it->name))
      cli->call_list.erase(it++);
    else
      ++it;
  }
  cli->checkCall();
  cli->fillPacketStruct();
  if (proto_map.find(cli->numQuery()) == proto_map.end())
    return (-1);
  return (static_cast<int>((this->*proto_map[cli->numQuery()])(cli)));
}

template<typename T>
bool ConversationController<T>::videoCallInitFct(UserManager<T> *cl) const {
  UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());

  if (!cl->isLogged())
    {
      cl->writeStruct({0, Enum::ENOT_LOGGED});
      return (true);
    }
  if (tmp_cl) {
    if (!(static_cast<int>(tmp_cl->getCallQuery()) ^
	  static_cast<int>(Enum::VIDEO_CALL_INIT))) {
      cl->writeStruct({ static_cast<uint16_t>(tmp_cl->getName().size()),
	    Enum::USER_VIDEO_HANG_UP });
      cl->writeMsg(tmp_cl->getName());
      return (true);
    }
    cl->writeStruct({0, Enum::OK});
    tmp_cl->writeStruct({static_cast<uint16_t>(cl->getName().size()),
	  Enum::INCOMING_VIDEO});
    tmp_cl->writeMsg(cl->getName());
    tmp_cl->addOnCallList(cl->getName(), cl, Enum::VIDEO_CALL_INIT);
  }
  else
    cl->writeStruct({0, Enum::ENO_EXIST});
  return (true);
}

template<typename T>
bool ConversationController<T>::userNotCalled(UserManager<T> *cl, UserManager<T> *tmp_cl) const {
  if (!tmp_cl)
    return (false);
  if (!cl->isLogged()) {
      cl->writeStruct({0, Enum::ENOT_LOGGED});
      return (true);
    }
  else if (!cl->deleteCallByName(tmp_cl->getName())) {
    cl->writeStruct({0, Enum::ENOT_CALLED});
    return (true);
  } 
  return (false);
}

template<typename T>
bool ConversationController<T>::audioCallAcceptFct(UserManager<T> *cl) const {
  UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());

  if (userNotCalled(cl, tmp_cl))
    return (true);
  else if (tmp_cl) {
      cl->writeStruct({0, Enum::OK});
    cl->writeStruct({static_cast<uint16_t>(tmp_cl->getIP().size() +
					   tmp_cl->getName().size() + 1),
	  Enum::AUDIO_START});
      cl->writeMsg(tmp_cl->getName() + ":" + tmp_cl->getIP());
      tmp_cl->writeStruct({static_cast<uint16_t>(cl->getName().size() + 1 +
						 cl->getIP().size()),
	    Enum::AUDIO_START});
      tmp_cl->writeMsg(cl->getName() + ":" + cl->getIP());
      cl->callWith(tmp_cl->getName(),
		   static_cast<Enum::ClientQueries>(static_cast<int>(cl->getCallQuery()) | 
					      static_cast<int>(Enum::AUDIO_CALL_INIT)));
      tmp_cl->callWith(cl->getName(),
		       static_cast<Enum::ClientQueries>(static_cast<int>(tmp_cl->getCallQuery()) |
							static_cast<int>(Enum::AUDIO_CALL_INIT)));
  }
  else
    cl->writeStruct({0, Enum::ENOT_CALLED});
  return (true);
}

template<typename T>
bool ConversationController<T>::videoCallAcceptFct(UserManager<T> *cl) const {
  UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());

  if (userNotCalled(cl, tmp_cl))
    return (true);
  else if (tmp_cl) {
    cl->writeStruct({0, Enum::OK});
    cl->writeStruct({static_cast<uint16_t>(tmp_cl->getIP().size() +
					   tmp_cl->getName().size() + 1),
	  Enum::VIDEO_START});
    cl->writeMsg(tmp_cl->getName() + ":" + tmp_cl->getIP());
    tmp_cl->writeStruct({static_cast<uint16_t>(cl->getName().size() + 1 +
					       cl->getIP().size()),
	  Enum::VIDEO_START});
    tmp_cl->writeMsg(cl->getName() + ":" + cl->getIP());
    cl->callWith(tmp_cl->getName(),
		 static_cast<Enum::ClientQueries>(static_cast<int>(cl->getCallQuery()) |
						  static_cast<int>(Enum::VIDEO_CALL_INIT)));
    tmp_cl->callWith(cl->getName(),
		     static_cast<Enum::ClientQueries>(static_cast<int>(tmp_cl->getCallQuery()) |
						      static_cast<int>(Enum::VIDEO_CALL_INIT)));
  }
  else
    cl->writeStruct({0, Enum::ENOT_CALLED});
  return (true);
}

template<typename T>
bool ConversationController<T>::audioCallRejectFct(UserManager<T> *cl) const {
  UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());

  if (userNotCalled(cl, tmp_cl))
    return (true);
  else if (tmp_cl) {
    cl->writeStruct({0, Enum::OK});
    tmp_cl->writeStruct({static_cast<uint16_t>(cl->getName().size()),
	  Enum::REJECT_AUDIO});
    tmp_cl->writeMsg(cl->getName());
  }
  else
    cl->writeStruct({0, Enum::ENOT_CALLED});
  return (true);
}

template<typename T>
bool ConversationController<T>::videoCallRejectFct(UserManager<T> *cl) const {
  UserManager<T> * tmp_cl = this->findUserByName(cl->getPacketData());

  if (userNotCalled(cl, tmp_cl))
    return (true);
  else if (tmp_cl) {
    cl->writeStruct({0, Enum::OK});
    tmp_cl->writeStruct({static_cast<uint16_t>(cl->getName().size()),
	  Enum::REJECT_VIDEO});
    tmp_cl->writeMsg(cl->getName());
  }
  else
    cl->writeStruct({0, Enum::ENOT_CALLED});
  return (true);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
template class ConversationController<SOCKET>;
#else
template class ConversationController<int>;
#endif
