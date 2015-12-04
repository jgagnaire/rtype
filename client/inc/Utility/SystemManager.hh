#ifndef SYSTEMMANAGER_HH_
# define SYSTEMMANAGER_HH_

#include "System/ISystem.hh"
#include "System/Render/RenderSystem.hh"
#include "System/Event/EventAggregator.hh"
#include "System/Utility/Clock.hh"

class SystemManager
{
public:
  SystemManager()
  {
    ISystem *render = new RenderSystem();
    systemList["render"] = render;
    ea = new EventAggregator(render.getWindow());
    clk = new Clock();
  }
  
  void gameLoop()
  {
    while (ea.getWin()->isOpen())
      {
	for (auto x : systemList)
	  {
	    x->second->update(this->clk);
	  }
      }
  }
  
private:
  std::unordered_map<ISystem*> systemList;
  EventAggregator	*ea;
  IClock		*clk;
};

#endif //SYSTEMMANAGER_HH_
