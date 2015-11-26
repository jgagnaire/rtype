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
        case Enum::PONG:
            return (static_cast<int>(pong(cli)));
        case Enum::UPLOAD_IMAGE:
            return (static_cast<int>(startUpload(cli)));
        case Enum::SEND_IMAGE:
            return (static_cast<int>(uploadImage(cli)));
        case Enum::IMAGE_UPLOADED:
            return (static_cast<int>(finishUpload(cli)));
        case Enum::DELETE_IMAGE:
            return (static_cast<int>(deleteImage(cli)));
        case Enum::RETRIEVE_IMAGE:
            return (static_cast<int>(retrieveImage(cli)));
        default:
            return (-1);
    }
}

template<typename T>
bool            AccountController<T>::uploadImage(UserManager<T> *cl) const {
    cl->writeStruct({0, cl->uploadImage()});
    return (true);
}

template<typename T>
bool            AccountController<T>::finishUpload(UserManager<T> *cl) const {
    cl->writeStruct({0, cl->endOfUploading()});
    return (true);
}

template<typename T>
bool            AccountController<T>::deleteImage(UserManager<T> *cl) const {
    cl->writeStruct({0, cl->deleteImage()});
    return (true);
}

template<typename T>
bool    AccountController<T>::startUpload(UserManager<T> *cl) const {
    cl->writeStruct({0, cl->startUpload()});
    return (true);
}

template<typename T>
bool    AccountController<T>::retrieveImage(UserManager<T> *cl) const {
    cl->writeStruct({0, cl->retrieveImage()});
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
    cl->writeStruct({0, cl->verifyUser()});
    return (true);
}

template<typename T>
bool    AccountController<T>::accountRegisterFct(UserManager<T> *cl) const {
    cl->writeStruct({0, cl->newUser()});
    return (true);
}

template<typename T>
bool	AccountController<T>::disconnectionFct(UserManager<T> *cl) const {
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
