#ifndef ICOMPENENT_H_
# define ICOMPENENT_H_

#include <string>
#include <Time.hpp>

class ICompenent
{
public:
  ICompenent(const std::string& type);
  virtual ~ICompenent();

  inline std::string typename(void) const {
    return (_type);
  }
  virtual void update(sf::Time fps) = 0;
protected:
  std::string	_type;
};

#endif //ICOMPENENT_H_
