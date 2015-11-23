#ifndef COMPENENTMANAGER_H_
# define COMPENENTMANAGER_H_

#include "Compenent.hh"
#include <unordered_map>

class CompenentManager
{

private:
  std::unordered_map<std::string, ICompenent*> compenents;
  
public:
  template<typename Type>
  void add(const std::string& name, Type value)
  {
    if (compenents.find(name) == compenents.end())
      {
	ICompenent	*tmp;
	tmp = new Compenent<Type>(name, value);
	compenents[name] = tmp;
      }
  }
  
  template<typename Type>
  void set(const std::string& name, Type value)
  {
    if (compenents.find(name) != compenents.end())
      {
	if (compenents[name]->getType() == typeid(Type).name())
	  static_cast<Compenent<Type>*>(compenents[name])->setValue(value);
      }
  }
  
  template<typename Type>
  Type get(const std::string& name)
  {
    if (compenents.find(name) != compenents.end())
      {
	if (compenents[name]->getType() == typeid(Type).name())
	  static_cast<Compenent<Type>*>(compenents[name])->getValue();
      }
  }
  
};

#endif //COMPENENTMANAGER_H_
