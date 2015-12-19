#ifndef MOBSYSTEM_HH_
# define MOBSYSTEM_HH_

#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"

class	MobSystem : public ASystem
{
    private:
        Entity *createMob(void)
        {
            Entity *e = new Entity;
            e->manager.add<std::string>("name", "mob1");
            e->manager.add<std::string>("type", "mob");
            e->manager.add<float>("velocity", 0.30f);
            e->manager.add("position",
                    std::pair<float, float>(1850, rand() % 1000));
            e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                ("pattern", Pattern::line);
            e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
            return e;
        }
    public:
        MobSystem() {}
        MobSystem(std::list<Entity*> *list) : isActiv(false), _eList(list) {
            _eList->push_back(createMob());
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
                        _eList->push_back(createMob());
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
};

#endif
