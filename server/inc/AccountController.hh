#pragma once
# include "AController.hh"

template<typename T>
class AccountController : public AController<T> {
public:
    AccountController(std::list<UserManager<T> *> *);

    virtual         ~AccountController() {}

    bool	        accountLoginFct(UserManager<T> *) const;
    bool	        accountRegisterFct(UserManager<T> *) const;
    bool	        disconnectionFct(UserManager<T> *) const;
    bool            startUpload(UserManager<T> *) const;
    bool            uploadImage(UserManager<T> *) const;
    bool            finishUpload(UserManager<T> *) const;
    bool            deleteImage(UserManager<T> *) const;
    bool            retrieveImage(UserManager<T> *) const;

    bool	        pong(UserManager<T> *) const;
    int		        newConnection(UserManager<T> *) const;
    int		        closeConnection(UserManager<T> *) const;
    virtual int	    newData(UserManager<T> *);
private:
	AccountController &operator=(AccountController const &rhs) { if (&rhs != this) {} return *this; }
};
