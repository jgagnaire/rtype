#ifndef ICOMPENENT_H_
# define ICOMPENENT_H_

#include <string>

class ICompenent
{
public:
  ICompenent(const std::string& type, const std::string& name) : _type(type), _name(name)
  {}
  
  virtual ~ICompenent()
  {
  }

  inline std::string getType(void) const {
    return (_type);
  }

  inline std::string getName(void) const {
    return (_name);
  }
  
  virtual void update(sf::Time fps) = 0;
protected:
  std::string	_type;
  std::string	_name;
};

#endif //ICOMPENENT_H_
