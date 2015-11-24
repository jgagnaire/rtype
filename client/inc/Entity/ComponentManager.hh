#ifndef COMPENENTMANAGER_H_
# define COMPENENTMANAGER_H_

#include "Component/Component.hh"
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
            }

        template<typename Type>
            void set(const std::string& name, Type value)
            {
                if (compenents.find(name) != compenents.end())
                {
                    if (compenents[name]->getType() == typeid(Type).name())
                        static_cast<Component<Type>*>(compenents[name])->setValue(value);
                }
            }

        template<typename Type>
            Type &get(const std::string& name)
            {
                if (compenents.find(name) != compenents.end())
                {
                    if (compenents[name]->getType() == typeid(Type).name())
                        return (static_cast<Component<Type>*>(compenents[name])->getValue());
                }
				throw;
            }

        template<typename Type>
            std::vector<Type>	getAll(void)
            {
                std::vector<Type> v;

                for (std::unordered_map<std::string, IComponent*>::iterator it = compenents.begin();
                        it != compenents.end();++it)
                {
                    if (it->second->getType() == typeid(Type).name())
                        v.push_back((static_cast<Component<Type>* >(it->second))->getValue());
                }
                // std::for_each(compenents.begin(), compenents.end(),
                // 		  [v](std::pair<std::string, IComponent*> cmp) {
                // 		    if (cmp.second->getType() == typeid(Type).name())
                // 		      {
                // 			Type  &cul = (static_cast<Component<Type>* >(cmp.second))->getValue();
                // 			std::cout <<
                // 			  static_cast<Component<Type>*>(cmp.second)->getValue() << std::endl;
                // 			v.push_back(&cul);
                // 		      }
                //   });
                return (v);
            }
};

#endif //COMPENENTMANAGER_H_
