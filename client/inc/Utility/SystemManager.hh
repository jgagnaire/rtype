#ifndef SYSTEMMANAGER_HH_
# define SYSTEMMANAGER_HH_

#include "System/ASystem.hh"
#include "System/Render/RenderSystem.hh"
#include "System/Event/EventAggregator.hh"
#include "System/Audio/AudioCallSystem.hh"
#include "System/Movement/Movement.hh"
#include "System/Shoot/ShootSystem.hh"
#include "System/Collider/ColliderSystem.hh"
#include "System/Mob/MobSystem.hh"
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
		ASystem *mob = new MobSystem(shr_entities);
		ASystem *col = new ColliderSystem(shr_entities);

        systemList["1mov"] = mvt;
        systemList["2mob"] = mob;
        systemList["3Shoot"] = shot;
        systemList["4col"] = col;
        systemList["5audioCall"] = audioCall;
        systemList["6render"] = render;
        ea = new EventAggregator(static_cast<RenderSystem*>(render)->getWindow());
        clk = new Clock();
        ea->add(mvt);
		ea->add(mob);
        ea->add(shot);
        ea->add(col);
		ea->add(audioCall);
        ea->add(render);
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
            EventSum        event;
            UdpPacket       lastEvent;
            std::string     tmp;

            lastEvent.setQuery(static_cast<uint16_t>(UdpCodes::KeyPressed));
            while (ea->getWin()->isOpen())
            {
                event = 0;
                std::size_t s = this->clk->getElapsedTimeMilli();
                this->clk->restart();
                ea->update();
                for (auto x : systemList)
                {
                    IPacket *m = x.second->out(event);
                    if (m != 0)
                        _networkManager.send(*m);
                    x.second->update(s);
                }
                tmp = std::to_string(event);
                lastEvent.setData(tmp.c_str());
                lastEvent.setSize(tmp.size());
                _networkManager.send(lastEvent);
                IPacket *p;
                while (dynamic_cast<UdpPacket*>(p = _networkManager.getPacket()))
                    if (p)
                    {
                        for (auto x : systemList)
                            x.second->in(p);
                        delete[] static_cast<const char*>(p->getData());
                        delete p;
                    }
                if (p)
                {
                    for (auto x : systemList)
                        x.second->in(p);
                    delete[] static_cast<const char*>(p->getData());
                    delete p;
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
