#ifndef COMPENENT_H_
# define COMPENENT_H_

#include "Component/IComponent.hh"
#include <typeinfo>

template<typename Type>
class Component : public IComponent
{
public:
  Component(const std::string& name, Type value) : IComponent(typeid(Type).name(), name)
  {
    _value = new Type(value);
  }

  virtual ~Component() {}

  inline void	setValue(Type &value) {
    _value = value;
  }

  inline Type& getValue(void) const {
    return (*_value);
  }

  virtual void update(std::chrono::steady_clock) {}
private:
  Type *_value;
};

#endif //COMPENENT_H_
