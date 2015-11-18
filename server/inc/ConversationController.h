#pragma once
# include "AController.h"

template<typename T>
class ConversationController : public AController<T> {
public:
  typedef bool	(ConversationController<T>::*proto_fptr)(UserManager<T> *) const;
  ConversationController(std::list<UserManager<T> *> &);

  virtual ~ConversationController() {}

  bool	sendTextFct(UserManager<T> *) const;
  bool	audioCallInitFct(UserManager<T> *) const;
  bool	videoCallInitFct(UserManager<T> *) const;
  bool	audioCallAcceptFct(UserManager<T> *) const;
  bool	videoCallAcceptFct(UserManager<T> *) const;
  bool	audioCallRejectFct(UserManager<T> *) const;
  bool	videoCallRejectFct(UserManager<T> *) const;
  bool	userNotCalled(UserManager<T> *, UserManager<T> *) const;
  bool	hangUpFct(UserManager<T> *) const;
  bool	hangUpVideoFct(UserManager<T> *) const;
  void	hangUp(UserManager<T> *cl,
	       Enum::ClientQueries cq,
	       Enum::ServerNotification type) const;
  virtual int	newData(UserManager<T> *cli);
private:
  std::map<Enum::ClientQueries, proto_fptr>	proto_map;
};
