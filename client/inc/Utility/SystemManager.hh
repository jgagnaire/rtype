#ifndef SYSTEMMANAGER_HH_
# define SYSTEMMANAGER_HH_

#include "System/ASystem.hh"
#include "System/Render/RenderSystem.hh"
#include "System/Event/EventAggregator.hh"
#include "System/Audio/AudioCallSystem.hh"
#include "System/Movement/Movement.hh"
#include "System/Shoot/ShootSystem.hh"
#include "System/Collider/ColliderSystem.hh"
#include "System/Sound/SoundSystem.hh"
#include "System/Mob/MobSystem.hh"
#include "Utility/Clock.hh"
#include "Network/NetworkManager.hh"

class SystemManager
{
    public:
        SystemManager(const std::string &ip):
            _networkManager(ip, ip), shr_entities(new std::unordered_map<std::size_t, Entity*>)
    {
        Entity  *last = new Entity;
        last->manager.add<std::size_t>("lastPlayer", 0);
        last->manager.add<std::size_t>("lastShoot", 1000000000);
        last->manager.add<std::size_t>("lastMob", 2000000000);
        last->manager.add<std::size_t>("lastBonus", 3000000000);
        last->manager.add<std::size_t>("lastMobShoot", 4000000000);
        last->manager.add<std::size_t>("lastBoss", 5000000000);
        last->manager.add<int>("changeDuration", 10000);
        last->manager.add<std::string>("type", "none");
        last->manager.add<std::string>("name", "none");
        last->manager.add<std::pair<float, float> >("position", std::pair<float,float>(0,0));
        last->manager.add<Pattern::Side>("direction", Pattern::Side::RIGHT);
        last->manager.add<Pattern::MovePattern>
            ("pattern", Pattern::MovePattern::LINE);
        last->manager.add<fCollision>("collision", &Collision::explosion);
        (*shr_entities)[-1] = last;
        ASystem *render = new RenderSystem(shr_entities);
        ASystem *audioCall = new AudioCallSystem();
        ASystem *mvt = new MovementSystem(shr_entities);
        ASystem *shot = new ShootSystem(shr_entities);
        ASystem *mob = new MobSystem(shr_entities);
        ASystem *col = new ColliderSystem(shr_entities);
        ASystem *sound = new SoundSystem();

        systemList.push_back(mvt);
        systemList.push_back(shot);
        systemList.push_back(col);
        systemList.push_back(mob);
        systemList.push_back(audioCall);
        systemList.push_back(sound);
        systemList.push_back(render);
        ea = new EventAggregator(static_cast<RenderSystem*>(render)->getWindow());
        ea->add(mvt);
        ea->add(mob);
        ea->add(shot);
        ea->add(col);
        ea->add(audioCall);
        ea->add(render);
        ea->add(sound);
    }

        ~SystemManager()
        {
            for(auto x : systemList)
                delete(x);
            delete ea;
        }

        void gameLoop()
        {
            EventSum        event;
            UdpPacket       lastEvent;
            std::string     tmp;
            std::size_t s = 0;
            auto start = std::chrono::steady_clock::now();
            auto end = std::chrono::steady_clock::now();

            if (_networkManager.isConnected() == false)
            {
                std::cerr << "Can't connect" << std::endl;
				return ;
            }
            lastEvent.setQuery(static_cast<uint16_t>(UdpCodes::KeyPressed));
            while (ea->getWin()->isOpen())
            {
                start = std::chrono::steady_clock::now();
                event = 0;
                std::chrono::duration<double> diff = start - end;
                s = static_cast<std::size_t>(diff.count() * 1000);
                if (s > 30)
                {
                    end = start;
                    ea->update();
                    for (auto x : systemList)
                    {
                        IPacket *m = x->out(event);
                        if (m != 0)
                            _networkManager.send(*m);
                        x->update(s);
                    }
                    tmp = std::to_string(event);
                    lastEvent.setData(tmp.c_str());
                    lastEvent.setSize(static_cast<uint16_t>(tmp.size()));
                    _networkManager.send(lastEvent);
                    IPacket *p;
                    while (dynamic_cast<UdpPacket*>(p = _networkManager.getPacket()))
                        if (p)
                        {
                            for (auto x : systemList)
                                x->in(p);
                            delete[] static_cast<const char*>(p->getData());
                            delete p;
                        }
                    if (p)
                    {
                        for (auto x : systemList)
                            x->in(p);
                        delete[] static_cast<const char*>(p->getData());
                        delete p;
                    }
                }
            }
        }

    private:
        std::vector<ASystem*>	systemList;
        EventAggregator								*ea;
        NetworkManager								_networkManager;
        std::unordered_map<std::size_t, Entity*>	*shr_entities;
};

#endif //SYSTEMMANAGER_HH_
