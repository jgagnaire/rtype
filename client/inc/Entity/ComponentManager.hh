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
        std::unordered_map<std::string, IComponent*> components;

    public:
        template<typename Type>
            void add(const std::string& name, Type value)
            {
                if (components.find(name) == components.end())
                {
                    IComponent	*tmp;
                    tmp = new Component<Type>(name, value);
                    components[name] = tmp;
                }
                else
                    throw ComponentManagerException("Component already exists");
            }

  template<typename Type>
  void set(const std::string& name, Type value)
  {
    if (components.find(name) != components.end())
      {
	if (components[name]->getType() == typeid(Type).name())
	  static_cast<Component<Type>*>(components[name])->setValue(value);
      }
    else
      throw ComponentManagerException("Component does not exist");
  }

  const std::string&	getType(const std::string& name)
  {
    if (components.find(name) != components.end())
      return (components[name]->getType());
    throw ComponentManagerException("No such component to get : [invalid name]");
  }
  
  template<typename Type>
  Type &get(const std::string& name)
  {
    if (components.find(name) != components.end())
      {
	if (components[name]->getType() == typeid(Type).name())
	  return (static_cast<Component<Type>*>(components[name])->getValue());
	else
	  throw ComponentManagerException("No such component to get : [invalid type] \
type for compenents \"" + name + "\" is : " + components[name]->getType());
      }
    throw ComponentManagerException("No such component to get : [invalid name]");
  }
  
  template<typename Type>
  bool	remove(const std::string& name = "")
  {
    if (!name.empty() && components.find(name) != components.end())
      {
	if (components[name]->getType() == typeid(Type).name())
	  {
	    components.erase(name);
	    return true;
	  }
	return false;
      }
    else if (name.empty()) {
      for (std::unordered_map<std::string, IComponent*>::iterator it
	     = components.begin();
	   it != components.end();++it)
	{
	  if (it->second->getType() == typeid(Type).name())
	    {
	      components.erase(it->first);
	      return true;
	    }
	}
      return false;
    }
    return false;
  }

	//Remove all compenents, destructor are called
	inline void	removeAll(void)
		{
			components.clear();
		}
  
  template<typename Type>
  std::vector<Type>	getAll(void)
  {
    std::vector<Type> v;
    
    for (std::unordered_map<std::string, IComponent*>::iterator it
	   = components.begin();
	 it != components.end();++it)
      {
	if (it->second->getType() == typeid(Type).name())
	  v.push_back((static_cast<Component<Type>* >(it->second))->getValue());
      }
    return (v);
  }
};

#endif //COMPENENTMANAGER_H_
