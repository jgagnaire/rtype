#ifndef MANAGEREXCEPTION_HH
#define MANAGEREXCEPTION_HH

#include <exception>
#include <stdexcept>
#include <string>

class ComponentManagerException : public std::runtime_error
{
public:
  ComponentManagerException(const char *msg) :
    runtime_error(msg) {}
  ComponentManagerException(const std::string &msg) :
    runtime_error(msg) {}
  virtual ~ComponentManagerException() throw() {}
};

#endif // MANAGEREXCEPTION_HH
