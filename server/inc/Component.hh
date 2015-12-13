#ifndef COMPENENT_H_
# define COMPENENT_H_

#include "IComponent.hh"
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

	inline void	setValue(Type value) { delete _value; _value = new Type(value); }

  inline Type& getValue(void) const { return (*_value); }

private:
  Type *_value;
};

#endif //COMPENENT_H_
