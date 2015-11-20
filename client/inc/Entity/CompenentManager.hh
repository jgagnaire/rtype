#ifndef COMPENENTMANAGER_H_
# define COMPENENTMANAGER_H_

#include "Compenent.h"
#include <unordered_map>

class CompenentManager
{
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
  void set(const std::string& name, Type value);
  {
    if (compenents.find(name) != compenents.end())
      {
	if (compenents[name].name() == typeid(Type).name)
	  static_cast<Compenent<TYPE>*>(compenents[name])->setValue(value);
      }
  }
  
  template<typename Type>
  Type get(const std::string& name)
  {
    if (compenents.find(name) != compenents.end())
      {
	if (compenents[name].name() == typeid(Type).name)
	  static_cast<Compenent<TYPE>*>(compenents[name])->getValue();
      }
  }
  
private:
  std::unordored_map<std::string, ICompenent*> compenents;
}

#endif //COMPENENTMANAGER_H_
