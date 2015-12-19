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
            // TODO REMOVE THE IFS
            if (_monsters.size())
                e->manager.add<float>("velocity", _monsters[name].manager.get<float>("velocity"));
            else
                e->manager.add<float>("velocity", 2.0f);
            e->manager.add<std::pair<float, float> >("position", pos);
            if (_monsters.size())
                e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                    ("pattern",
                     Pattern::getPattern(
                         _monsters[name].manager.get<std::string>("movement")));
            else
                e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                    ("pattern", &Pattern::line);
            e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
            return e;
        }
    public:
        MobSystem() {}
        MobSystem(std::list<Entity*> *list) : isActiv(false), _eList(list) {
            std::pair<float, float> p(1920, 500);
            _eList->push_back(createMob("mob2", p));
            _eventList.push_back(E_Stage);
        }
        virtual ~MobSystem() {}

        virtual void                    update(int duration)
        {
            if (!isActiv)
                return ;
            bool has_been_del = false;
            for (auto x = _eList->begin(); x != _eList->end();)
            {
                has_been_del = false;
                if ((*x)->manager.get<std::string>("type") == "mob")
                {
                    (*x)->manager.get<std::function<void (Entity&, Pattern::Side, int)> >
                        ("pattern")(**x, (*x)->manager.
                                    get<Pattern::Side>("direction"), duration);
                    std::pair<float, float> tmp = (*x)->manager.
                        get<std::pair<float, float> >("position");
                    if (tmp.first <= 0)
                    {
                        x = _eList->erase(x);
                        std::pair<float, float> p(1920, 500);
                        _eList->push_back(createMob("mob2", p));
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
            if ((packet = dynamic_cast<TcpPacket*>(p)))
            {
                std::string tmp = std::string(static_cast<const char *>(p->getData()), p->getSize());
                if (p->getQuery() == static_cast<uint16_t>(Codes::JsonMonsters))
                {
                    Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("monsters");
                    for (auto &x : e.manager.getAll<Entity>())
                        _monsters[x.first] = x.second;
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
                                    tmp->manager.add<int>("appearIn", firstTime + interval * i);
                                }
                                l.push_back(tmp);
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
