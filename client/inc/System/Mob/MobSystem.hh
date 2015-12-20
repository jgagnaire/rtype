#ifndef MOBSYSTEM_HH_
# define MOBSYSTEM_HH_

#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"

class	MobSystem : public ASystem
{
    private:
        Entity *createMob(const std::string &name, const std::pair<float, float> &pos)
        {
            Entity *e = new Entity;
            e->manager.add<std::string>("name", name);
            e->manager.add<std::string>("type", "mob");
            e->manager.add<float>("velocity", _monsters[name].manager.get<float>("velocity"));
            e->manager.add<std::pair<float, float> >("position", pos);
            e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                ("pattern",
                 Pattern::getPattern(
                     _monsters[name].manager.get<std::string>("movement")));
            e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
            return e;
        }

        Entity *createBonus(const std::string &name, const std::pair<float, float> &pos)
        {
            Entity *e = new Entity;
            e->manager.add<std::string>("name", name);
            e->manager.add<std::string>("type", "bonus");
            e->manager.add<float>("velocity", _bonuses[name].manager.get<float>("velocity"));
            e->manager.add<std::pair<float, float> >("position", pos);
            e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                ("pattern",
                 Pattern::getPattern(
                     _bonuses[name].manager.get<std::string>("movement")));
            e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
            return e;
        }

    public:
        MobSystem() {}
        MobSystem(std::list<Entity*> *list) : isActiv(false), _eList(list) {
            _eventList.push_back(E_Stage);
        }
        virtual ~MobSystem() {}

        virtual void                    update(int duration)
        {
            if (!isActiv)
                return ;
            for (auto x = _waitingmobs[0].begin(); x != _waitingmobs[0].end();
                    ++x)
            {
                int tmp = (*x)->manager.get<int>("appearIn");
                tmp -= duration;
                (*x)->manager.set<int>("appearIn", tmp);
                if (tmp <= 0)
                {
                    _eList->push_back(*x);
                    x = _waitingmobs[0].erase(x);
                    --x;
                }
            }
            for (auto x = _eList->begin(); x != _eList->end(); ++x)
            {
                if ((*x)->manager.get<std::string>("type") == "mob")
                {
                    (*x)->manager.get<std::function<void (Entity&, Pattern::Side, int)> >
                        ("pattern")(**x, (*x)->manager.
                                    get<Pattern::Side>("direction"), duration);
                }
            }
        }

        virtual IPacket                 *out(EventSum&) { return 0;}

        virtual void                    in(IPacket *p)
        {
            TcpPacket       *packet;
            if ((packet = dynamic_cast<TcpPacket*>(p)))
            {
                std::string tmp = std::string(static_cast<const char *>(p->getData()), p->getSize());
                if (p->getQuery() == static_cast<uint16_t>(Codes::JsonMonsters))
                {
                    Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("monsters");
                    for (auto &x : e.manager.getAll<Entity>())
                        _monsters[x.first] = x.second;
                }
                else if (p->getQuery() == static_cast<uint16_t>(Codes::JsonBonuses))
                {
                    Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("bonuses");
                    for (auto &x : e.manager.getAll<Entity>())
                        _bonuses[x.first] = x.second;
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
                                std::pair<float, float> pair(pos.manager.get<int>("x"),
                                        pos.manager.get<int>("y"));
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
                                int     numbers;
                                int     firstTime;
                                int     interval;

                                Entity *tmp;
                                Entity &pos = bonus.second.manager.get<Entity>("position");
                                std::pair<float, float> pair(pos.manager.get<int>("x"),
                                        pos.manager.get<int>("y"));
                                numbers = bonus.second.manager.get<int>("time");
                                firstTime = bonus.second.manager.get<int>("appear_at_sec");
                                interval = bonus.second.manager.get<int>("timeval");
                                for (int i = 0; i < numbers; ++i)
                                {
                                    tmp = createBonus(bonus.first, pair);
                                    tmp->manager.add<int>("appearIn", firstTime * 1000 + interval * i);
                                    l.push_back(tmp);
                                }
                            }
                        }
                        _waitingmobs.push_back(l);
                    }
                }
            }
        }
        virtual bool                    handle(EventSum ev)
        {
            if (ev == E_Stage)
                isActiv = !isActiv;
            return true;
        }
        virtual std::vector<REvent>     &broadcast(void) { return _eventList; }
        virtual EventSum                getEvent() {return noEvent;}

    protected:
        bool                                        isActiv;
        std::list<Entity*>                          *_eList;
        std::unordered_map<std::string, Entity>     _monsters;
        std::vector<std::list<Entity*> >            _waitingmobs;
};

#endif
