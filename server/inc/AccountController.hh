#pragma once
# include "AController.hh"

template<typename T>
class AccountController : public AController<T> {
public:
  typedef bool	(AccountController<T>::*proto_fptr)(UserManager<T> *) const;
  AccountController(std::list<UserManager<T> *> *);

  virtual ~AccountController() {}

  bool	changeStatus(UserManager<T> *) const;
  bool	accountLoginFct(UserManager<T> *) const;  
  bool	accountRegisterFct(UserManager<T> *) const;
  bool	disconnectionFct(UserManager<T> *) const;

  bool	        pong(UserManager<T> *) const;
  int		newConnection(UserManager<T> *) const;
  int		closeConnection(UserManager<T> *) const;
  virtual int	newData(UserManager<T> *);
};
