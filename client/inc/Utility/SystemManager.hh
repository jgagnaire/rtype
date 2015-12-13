#ifndef SYSTEMMANAGER_HH_
# define SYSTEMMANAGER_HH_

#include "System/ASystem.hh"
#include "System/Render/RenderSystem.hh"
#include "System/Event/EventAggregator.hh"
#include "System/Audio/AudioCallSystem.hh"
#include "System/Movement/Movement.hh"
#include "Utility/Clock.hh"
#include "Network/NetworkManager.hh"

class SystemManager
{
public:
	SystemManager(const std::string &ip):
		_networkManager(ip, ip), shr_entities(new std::list<Entity*>)
        {
			Entity *e = new Entity;
			e->manager.add<std::string>("name", "player1");
			e->manager.add<std::string>("type", "player");
			e->manager.add("position", std::pair<float, float>(0, 0));
			e->manager.add<bool>("isShared", true);
			shr_entities->push_back(e);

			ASystem *render = new RenderSystem(shr_entities);
			ASystem *audioCall = new AudioCallSystem();
			ASystem *mvt = new MovementSystem(shr_entities);
			
			systemList["1mov"] = mvt;
			systemList["3render"] = render;
			systemList["2audioCall"] = audioCall;
            ea = new EventAggregator(static_cast<RenderSystem*>(render)->getWindow());
            clk = new Clock();
            ea->add(render);
			ea->add(mvt);
        }

	~SystemManager()
		{
			for(auto x : systemList)
				delete(x.second);
			delete ea;
			delete clk;
		}

	void gameLoop()
        {
            while (ea->getWin()->isOpen())
            {
                std::size_t s = this->clk->getElapsedTimeMilli();
                this->clk->restart();
                IPacket *p = _networkManager.getPacket();
                for (auto x : systemList)
                {
                    if (p)
                        x.second->in(p);
                    x.second->update(s);
                    IPacket *m = x.second->out();
                    if (m != 0)
                        _networkManager.send(*m);
                }				
				ea->update();
            }
        }

private:
    std::unordered_map<std::string, ASystem*>	systemList;
    EventAggregator								*ea;
    IClock										*clk;
    NetworkManager								_networkManager;
    std::list<Entity*>							*shr_entities;
};

#endif //SYSTEMMANAGER_HH_
