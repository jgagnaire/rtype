#ifndef COMPONENT_H_
# define COMPONENT_H_

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

  Component(const Component &c) {
    if (this != &c)
      {
        _value = new Type(c._value);
      }    
  }

  virtual IComponent	*clone() { return (new Component(_name, *_value)); }

  Component&	operator=(const Component &c)
  {
    if (this != &c)
      {
	delete _value;
        _value = new Type(c._value);
      }
    return (*this);
  }

  virtual ~Component() {
    delete _value;
  }

	inline void	setValue(Type value) { delete _value; _value = new Type(value); }

  inline Type& getValue(void) const { return (*_value); }

private:
  Type *_value;
};

#endif //COMPONENT_H_
