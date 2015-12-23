#ifndef MOBSYSTEM_HH_
# define MOBSYSTEM_HH_

#include <list>
#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"
#include "System/Collider/FCollision.hh"

class	MobSystem : public ASystem
{
    private:
        Entity *createMob(const std::string &name, const std::pair<float, float> &pos)
        {
            Entity *e = new Entity;
            *e = _jsonEntities[name];
            e->manager.add<std::string>("name", name);
            e->manager.add<std::string>("type", "mob");
            e->manager.add<std::pair<float, float> >("position", pos);
            e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                ("pattern",
                 Pattern::getPattern(
                     _jsonEntities[name].manager.get<std::string>("movement")));
            e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
            e->manager.add<fCollision>("collision", &Collision::mob);
            return e;
        }

        Entity *createBonus(const std::string &name, const std::pair<float, float> &pos)
        {
            Entity *e = new Entity;
            *e = _jsonEntities[name];
            e->manager.add<std::string>("name", name);
            e->manager.add<std::string>("type", "bonus");
            e->manager.add<std::pair<float, float> >("position", pos);
            e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                ("pattern",
                 Pattern::getPattern(
                     _jsonEntities[name].manager.get<std::string>("movement")));
            e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
            e->manager.add<fCollision>("collision", &Collision::bonus);
            return e;
        }

    public:
        MobSystem() {}
        MobSystem(std::unordered_map<uint64_t, Entity*> *list) : isActiv(false), _eList(list), _lvl(1) {
            _eventList.push_back(E_Stage);
            _eventList.push_back(E_Ready);
            _durationAnimation = 0;
            _event = noEvent;
        }
        virtual ~MobSystem() {}

        virtual void                    update(int duration)
        {
            if (_durationAnimation > 0)
            {
                _durationAnimation -= duration;
                _durationAnimation = (_durationAnimation <= 0 ? 0 : _durationAnimation);
                return ;
            }
            if (!isActiv)
                return ;
            for (auto x = _waitingmobs[_lvl].begin(); x != _waitingmobs[_lvl].end();)
            {
                int tmp = (*x)->manager.get<int>("appearIn");
                tmp -= duration;
                (*x)->manager.set<int>("appearIn", tmp);
                if (tmp <= 0)
                {
                    if ((*x)->manager.get<std::string>("type") == "mob")
                    {
                        (*_eList)[(*_eList)[-1]->manager.get<uint64_t>("lastMob")] = *x;
                        (*x)->manager.add<int>("canIShoot", (*x)->manager.get<int>("fire_rate"));
                        (*_eList)[-1]->manager.set<uint64_t>("lastMob", (*_eList)[-1]->manager.get<uint64_t>("lastMob") + 1);
                    }
                    else if ((*x)->manager.get<std::string>("type") == "bonus")
                    {
                        (*_eList)[(*_eList)[-1]->manager.get<uint64_t>("lastBonus")] = *x;
                        (*_eList)[-1]->manager.set<uint64_t>("lastBonus", (*_eList)[-1]->manager.get<uint64_t>("lastBonus") + 1);
                    }
                    else if ((*x)->manager.get<std::string>("type") == "boss")
                    {
                        (*_eList)[(*_eList)[-1]->manager.get<uint64_t>("lastBoss")] = *x;
                        (*x)->manager.add<int>("canIShoot", (*x)->manager.get<int>("fire_rate"));
                        (*_eList)[-1]->manager.set<uint64_t>("lastBoss", (*_eList)[-1]->manager.get<uint64_t>("lastBoss") + 1);
                    }
                    x = _waitingmobs[_lvl].erase(x);
                }
                else
                    ++x;
            }
            bool has_been_del;
            for (auto x = _eList->begin(); x != _eList->end();)
            {
                has_been_del = false;
                if ((*x).second->manager.get<std::string>("type") == "mob"
                        || (*x).second->manager.get<std::string>("type") == "bonus"
                        || (*x).second->manager.get<std::string>("type") == "boss")
                {
                    if ((*x).second->manager.get<std::string>("type") == "mob"
                            || (*x).second->manager.get<std::string>("type") == "boss")
                    {
                        (*x).second->manager.set<int>("canIShoot", (*x).second->manager.get<int>("fire_rate") + duration);
                        if ((*x).second->manager.get<int>("canIShoot") >= (*x).second->manager.get<int>("fire_rate"))
                            for (auto shoot : (*x).second->manager.get<std::vector<std::string> >("fire"))
                            {
                                Entity *bullet = new Entity(_mobFires[shoot]);
                                uint64_t tmp = (*_eList)[-1]->manager.get<uint64_t>("lastMobShoot");
                                (*x).second->manager.set<int>("canIShoot", 0);
                                bullet->manager.add<std::pair<float, float> >("position",
                                        (*x).second->manager.get<std::pair<float, float> >("position"));
                                (*_eList)[tmp] = bullet;
                                ++tmp;
                                (*_eList)[-1]->manager.set<uint64_t>("lastMobShoot", tmp);
                            }
                    }

                    (*x).second->manager.get<std::function<void (Entity&, Pattern::Side, int)> >
                        ("pattern")(*((*x).second), (*x).second->manager.
                                    get<Pattern::Side>("direction"), duration);
                    auto tmp = (*x).second->manager.get<std::pair<float, float> >("position");
                    if (tmp.first < -1920 * 100)
                    {
                        delete (*x).second;
                        x = _eList->erase(x);
                        has_been_del = true;
                    }
                }
                if (!has_been_del)
                    ++x;
            }
        }

        virtual IPacket                 *out(EventSum&) { return 0;}

        virtual void                    in(IPacket *p)
        {
            TcpPacket       *packet;
            UdpPacket       *up;

            if ((up = dynamic_cast<UdpPacket*>(p)) &&
                    up->getQuery() == static_cast<uint16_t>(UdpCodes::Collided))
            {
                std::string data = std::string(
                        static_cast<const char*>(p->getData()),
                        p->getSize());
                uint64_t id1 = std::stoll(data.substr(0, data.find(":")));
                uint64_t id2 = std::stoll(data.substr(data.find(":") + 1));
                uint64_t boss = (id1 > id2 ? id1 : id2);
                if (boss + 1 == (*_eList)[-1]->manager.get<uint64_t>("lastBoss") &&
                        _eList->find(boss) == _eList->end())
                {
                    std::cout << "FINISH" << std::endl;
                    for (auto x = _eList->begin(); x != _eList->end();)
                    {
                        if (x->first >= 1000000000 && x->first != static_cast<uint64_t>(-1))
                        {
                            delete x->second;
                            x = _eList->erase(x);
                        }
                        else if (x->first < 1000000000)
                        {
                            std::pair<float, float> pos(0, 1080 / 2);
                            x->second->manager.set<int>("perfect_shield", 0);
                            x->second->manager.set<int>("respawn", 0);
                            x->second->manager.set<std::pair<float, float> >("position", pos);
                            ++x;
                        }
                        else
                            ++x;
                    }
                    ++_lvl;
                    _event = NewStage;
                    _durationAnimation = (*_eList)[-1]->manager.get<int>("changeDuration");
                }
            }
            if ((packet = dynamic_cast<TcpPacket*>(p)))
            {
                std::string tmp = std::string(static_cast<const char *>(p->getData()), p->getSize());
                if (p->getQuery() == static_cast<uint16_t>(Codes::JsonMonsters))
                {
                    Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("monsters");
                    for (auto &x : e.manager.getAll<Entity>())
                    {
                        _jsonEntities[x.first] = x.second;
                    }
                }
                else if (p->getQuery() == static_cast<uint16_t>(Codes::JsonBonuses))
                {
                    Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("bonuses");
                    for (auto &x : e.manager.getAll<Entity>())
                        _jsonEntities[x.first] = x.second;
                }

                else if (p->getQuery() == static_cast<uint16_t>(Codes::JsonShoots))
                {
                    Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("fires");
                    for (auto &x : e.manager.getAll<Entity>())
                    {
                        x.second.manager.add<std::string>("name", "mobShoot");
                        x.second.manager.add<std::string>("type", "mobshoot");
                        x.second.manager.add<fCollision>("collision", Collision::mobShoot);
                        x.second.manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
                        x.second.manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                            ("pattern", Pattern::getPattern(x.first));
                        _mobFires[x.first] = x.second;
                    }
                }
                else if (p->getQuery() == static_cast<uint16_t>(Codes::JsonLevels))
                {
                    Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("levels");
                    for (auto &main : e.manager.getAll<Entity>())
                    {
                        std::list<Entity*>  l;

                        for (auto &monsters : main.second.manager.get<std::vector<Entity> >("monsters"))
                        {
                            for (auto &monster : monsters.manager.getAll<Entity>())
                            {
                                int     numbers;
                                int     firstTime;
                                int     interval;

                                Entity *tmp;
                                Entity &pos = monster.second.manager.get<Entity>("position");
                                std::pair<float, float> pair(pos.manager.get<float>("x"),
                                        pos.manager.get<float>("y"));
                                numbers = monster.second.manager.get<int>("time");
                                firstTime = monster.second.manager.get<int>("appear_at_sec");
                                interval = monster.second.manager.get<int>("timeval");
                                for (int i = 0; i < numbers; ++i)
                                {
                                    tmp = createMob(monster.first, pair);
                                    tmp->manager.add<int>("appearIn", firstTime * 1000 + interval * i);
                                    l.push_back(tmp);
                                }
                            }
                        }
                        for (auto &bonuses: main.second.manager.get<std::vector<Entity> >("bonuses"))
                        {
                            for (auto &bonus: bonuses.manager.getAll<Entity>())
                            {
                                int     firstTime;

                                Entity *tmp;
                                Entity &pos = bonus.second.manager.get<Entity>("position");
                                std::pair<float, float> pair(pos.manager.get<float>("x"),
                                        pos.manager.get<float>("y"));
                                firstTime = bonus.second.manager.get<int>("appear_at_sec");
                                tmp = createBonus(bonus.first, pair);
                                tmp->manager.add<int>("appearIn", firstTime * 1000);
                                l.push_back(tmp);
                            }
                        }
                        Entity &tmp = main.second.manager.get<Entity>("boss");
                        Entity &pos = tmp.manager.get<Entity>("position");
                        std::pair<float, float> pair(pos.manager.get<float>("x"),
                                pos.manager.get<float>("y"));
                        Entity *boss = createMob(tmp.manager.get<std::string>("name"), pair);
                        boss->manager.add<int>("appearIn", main.second.manager.get<int>("time") * 1000);
                        boss->manager.set<std::string>("type", "boss");
                        l.push_back(boss);
                        int nb = std::stoi(main.first.substr(5));
                        _waitingmobs[nb] = l;
                    }
                }
            }
        }
        virtual bool                    handle(EventSum ev)
        {
            if (ev == E_Ready)
            {
                _jsonEntities.clear();
                for (auto &x : _waitingmobs)
                {
                    for (auto y : x.second)
                    {
                        delete y;
                    }
                }
                _waitingmobs.clear();
                isActiv = false;
                _lvl = 1;
            }
            if (ev == E_Stage)
                isActiv = true;
            return true;
        }
        virtual std::vector<REvent>     &broadcast(void) { return _eventList; }
        virtual EventSum                getEvent() {
            if (_event != noEvent)
            {
                EventSum tmp = _event;
                _event = noEvent;
                return tmp;
            }
            return _event;
        }

    protected:
        bool                                                    isActiv;
        std::unordered_map<uint64_t, Entity*>			*_eList;
        std::unordered_map<std::string, Entity>                 _jsonEntities;
        std::unordered_map<int, std::list<Entity*> >            _waitingmobs;
        std::unordered_map<std::string, Entity>			_mobFires;

        int                                                     _lvl;
        EventSum                                                _event;
        int                                                     _durationAnimation;
};

#endif
