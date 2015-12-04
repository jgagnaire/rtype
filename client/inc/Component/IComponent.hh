#ifndef ICOMPENENT_H_
# define ICOMPENENT_H_

#include <chrono>
#include <string>

class IComponent
{
public:
  IComponent(const std::string& type, const std::string& name) : _type(type), _name(name)
  {}

  virtual ~IComponent()
  {
  }

  inline std::string getType(void) const {
    return (_type);
  }

  inline std::string getName(void) const {
    return (_name);
  }

  virtual void update(std::chrono::steady_clock) = 0;
protected:
  std::string	_type;
  std::string	_name;
};

#endif //ICOMPENENT_H_
