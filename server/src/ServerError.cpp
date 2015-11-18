#include "ServerError.h"

ServerError::ServerError(eErrorType e)
{
  this->buildString(e);
}

ServerError::~ServerError() throw()
{
}

const char *ServerError::what() const throw()
{
  return this->s.str().c_str();
}

std::stringstream const &ServerError::getS() const
{
  return this->s;
}

void ServerError::buildString(eErrorType e)
{
  std::string tab[] =
    {
      "could not create and initialize socket",
      "could not accept incoming connection",
      "could not read from socket",
      "could not write to socket",
      "could not dynamic cast the ISocketSet into IServerSocket",
      "could not create event for socket",
      "could not select event for socket",
      "could not monitor sockets",
    };
  this->s << "Error: ";
  this->s << tab[e];
}
