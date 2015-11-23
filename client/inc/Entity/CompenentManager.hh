#ifndef COMPENENTMANAGER_H_
# define COMPENENTMANAGER_H_

#include "Compenent/Compenent.hh"
#include <unordered_map>
#include <vector>

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
  Type &get(const std::string& name)
  {
    if (compenents.find(name) != compenents.end())
      {
	if (compenents[name]->getType() == typeid(Type).name())
	  return (static_cast<Compenent<Type>*>(compenents[name])->getValue());
      }
  }

  template<typename Type>
  std::vector<Type*>	getAll(void)
  {
    std::vector<Type*> v;

    for (std::unordered_map<std::string, ICompenent*>::iterator it = compenents.begin();
	 it != compenents.end();++it)
      {
	std::cout << (static_cast<Compenent<Type>* >(it->second))->getName() << " : "<< (static_cast<Compenent<Type>* >(it->second))->getValue() << std::endl;
	v.push_back(&(static_cast<Compenent<Type>* >(it->second))->getValue());
      }
    // std::for_each(compenents.begin(), compenents.end(),
    // 		  [v](std::pair<std::string, ICompenent*> cmp) {
    // 		    if (cmp.second->getType() == typeid(Type).name())
    // 		      {
    // 			Type  &cul = (static_cast<Compenent<Type>* >(cmp.second))->getValue();
    // 			std::cout <<
    // 			  static_cast<Compenent<Type>*>(cmp.second)->getValue() << std::endl;
    // 			v.push_back(&cul);
    // 		      }
    //   });
    return (v);
  }  
};

#endif //COMPENENTMANAGER_H_
