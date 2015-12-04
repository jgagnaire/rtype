#ifndef COMPENENTMANAGER_H_
# define COMPENENTMANAGER_H_

#include "Component/Component.hh"
#include "Entity/ManagerException.hh"
#include <unordered_map>
#include <vector>
#include <iostream>

class ComponentManager
{

    private:
        std::unordered_map<std::string, IComponent*> compenents;

    public:
        template<typename Type>
            void add(const std::string& name, Type value)
            {
                if (compenents.find(name) == compenents.end())
                {
                    IComponent	*tmp;
                    tmp = new Component<Type>(name, value);
                    compenents[name] = tmp;
                }
                else
                    throw ComponentManagerException("Component already exist");
            }

  template<typename Type>
  void set(const std::string& name, Type value)
  {
    if (compenents.find(name) != compenents.end())
      {
	if (compenents[name]->getType() == typeid(Type).name())
	  static_cast<Component<Type>*>(compenents[name])->setValue(value);
      }
    else
      throw ComponentManagerException("Component doesn't exist");
  }
  
  template<typename Type>
  Type &get(const std::string& name)
  {
    if (compenents.find(name) != compenents.end())
      {
	if (compenents[name]->getType() == typeid(Type).name())
	  return (static_cast<Component<Type>*>(compenents[name])->getValue());
	else
	  throw ComponentManagerException("No such component to get : [invalid type] \
type for compenents \"" + name + "\" is : " + compenents[name]->getType());
      }
    throw ComponentManagerException("No such component to get : [invalid name]");
  }
  
  template<typename Type>
  bool	remove(const std::string& name = "")
  {
    if (!name.empty() && compenents.find(name) != compenents.end())
      {
	if (compenents[name]->getType() == typeid(Type).name())
	  {
	    compenents.erase(name);
	    return true;
	  }
	return false;
      }
    else { 
      for (std::unordered_map<std::string, IComponent*>::iterator it
	     = compenents.begin();
	   it != compenents.end();++it)
	{
	  if (it->second->getType() == typeid(Type).name())
	    {
	      compenents.erase(it->first);
	      return true;
	    }
	}
      return false;
    }
  }
  
  template<typename Type>
  std::vector<Type>	getAll(void)
  {
    std::vector<Type> v;
    
    for (std::unordered_map<std::string, IComponent*>::iterator it
	   = compenents.begin();
	 it != compenents.end();++it)
      {
	if (it->second->getType() == typeid(Type).name())
	  v.push_back((static_cast<Component<Type>* >(it->second))->getValue());
      }
    return (v);
  }
};

#endif //COMPENENTMANAGER_H_
