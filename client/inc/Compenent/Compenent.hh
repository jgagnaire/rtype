#ifndef COMPENENT_H_
# define COMPENENT_H_

#include "Compenent/ICompenent.hh"
#include <typeinfo>

template<typename Type>
class Compenent : public ICompenent
{
public:
  Compenent(const std::string& name, Type value) : ICompenent(typeid(Type).name(), name) 
  {
    _value = new Type(value);
  }
  
  virtual ~Compenent() {}

  inline void	setValue(Type &value) {
    _value = value;
  }

  inline Type& getValue(void) const {
    return (*_value);
  }

  virtual void update(sf::Time) {}
private:
  Type *_value;
};

#endif //COMPENENT_H_
