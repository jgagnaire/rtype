#ifndef SYSTEMMANAGER_HH_
# define SYSTEMMANAGER_HH_

#include "System/ASystem.hh"
#include "System/Render/RenderSystem.hh"
#include "System/Event/EventAggregator.hh"
#include "System/Audio/AudioCallSystem.hh"
#include "System/Movement/Movement.hh"
#include "System/Shoot/ShootSystem.hh"
#include "Utility/Clock.hh"
#include "Network/NetworkManager.hh"

class SystemManager
{
public:
	SystemManager(const std::string &ip):
		_networkManager(ip, ip), shr_entities(new std::list<Entity*>)
        {
			Entity *e = new Entity;
			e->manager.add<std::string>("pseudo", "");
			e->manager.add<std::string>("name", "player1");
			e->manager.add<std::string>("type", "player");
			e->manager.add("position", std::pair<float, float>(0, 0));
			e->manager.add<float>("velocity", 1.75f);
			e->manager.add<bool>("isShared", true);
			e->manager.add<Pattern::MovePattern>("pattern", Pattern::MovePattern::LINE);
			shr_entities->push_back(e);

			ASystem *render = new RenderSystem(shr_entities);
			ASystem *audioCall = new AudioCallSystem();
			ASystem *mvt = new MovementSystem(shr_entities);
			ASystem *shot = new ShootSystem(shr_entities);

			systemList["1mov"] = mvt;
			systemList["2Shoot"] = shot;
			systemList["3audioCall"] = audioCall;
			systemList["4render"] = render;
            ea = new EventAggregator(static_cast<RenderSystem*>(render)->getWindow());
            clk = new Clock();
            ea->add(render);
			ea->add(mvt);
			ea->add(shot);
            ea->add(audioCall);
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
                ea->update();
                for (auto x : systemList)
                {
                    IPacket *m = x.second->out();
                    if (m != 0)
                        _networkManager.send(*m);
                    x.second->update(s);
                }
                IPacket *p;
				while ((p = _networkManager.getPacket()))
				{
					for (auto x : systemList)
						if (p)
							x.second->in(p);
				}
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
