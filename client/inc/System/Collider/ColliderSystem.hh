#ifndef COLLIDER_HH_
# define COLLIDER_HH_

#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"
#include "Utility/JSONParser.hh"

class ColliderSystem : public ASystem
{
    public:
        ColliderSystem() {}
        ColliderSystem(std::list<Entity*> *list) :
            _isActiv(false), _eList(list) {
                _eventList.push_back(E_Stage);
            }
        virtual ~ColliderSystem() {}


        Entity          *createExplosion(const std::pair<float, float> &pos)
        {
            Entity *e = new Entity;
            e->manager.add<std::string>("name", "explosion");
            e->manager.add<std::string>("type", "explosion");
            e->manager.add<float>("velocity", 0.0f);
            e->manager.add<std::pair<float, float> >("position", pos);
            e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                ("pattern", Pattern::line);
            e->manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
            e->manager.add<fCollision>("collision", &Collision::explosion);
            _event = E_Explosion;
            return e;
        }

        void                            setSize(std::pair<int, int> &s,
                const std::string &name)
        {
            if (_hitboxes.find(name) != _hitboxes.end())
            {
                s.first = _hitboxes[name].first;
                s.second = _hitboxes[name].second;
            }
            else
            {
                s.first = 0;
                s.second = 0;
            }
        }

        virtual void                    update(int duration)
        {
            std::pair<float, float> p1, p2;
            std::pair<int, int>     s1, s2;
            std::string             t1, t2;
            Pattern::Side			d1, d2;
            bool					has_been_del = false;

            if (_isActiv)
            {
                for (auto a = _eList->begin(); a != _eList->end();)
                {
                    has_been_del = false;
                    p1 = (*a)->manager.get<std::pair<float, float> >("position");
                    t1 = (*a)->manager.get<std::string>("type");
                    d1 = (*a)->manager.get<Pattern::Side>("direction");
                    setSize(s1, (*a)->manager.get<std::string>("name"));
                    int time;
                    if (t1 == "player" && (time = (*a)->manager.get<int>("perfect_shield")) > 0)
                    {
                        time -= duration;
                        if (time < 0)
                            time = 0;
                        (*a)->manager.set<int>("perfect_shield", time);
                    }
                    for (auto b = _eList->begin(); b != _eList->end(); ++b)
                    {
                        if (*a != *b)
                        {
                            p2 = (*b)->manager.get<std::pair<float, float> >("position");
                            t2 = (*b)->manager.get<std::string>("type");
                            d2 = (*b)->manager.get<Pattern::Side>("direction");
                            setSize(s2, (*b)->manager.get<std::string>("name"));
                            if (t1 != t2 && d1 != d2 && p1.first < p2.first + s2.first &&
                                    p1.first + s1.first > p2.first &&
                                    p1.second < p2.second + s2.second &&
                                    s1.second + p1.second > p2.second)
                            {
                                std::pair<float, float> ex(-1, -1);
                                bool delA = (*a)->manager.get<fCollision>("collision")(**a, **b, ex);
                                if (ex.first > -1)
                                    _eList->push_back(createExplosion(ex));
                                ex.first = ex.second = -1;
                                bool delB = (*b)->manager.get<fCollision>("collision")(**b, **a, ex);
                                if (ex.first > -1)
                                    _eList->push_back(createExplosion(ex));
                                if (delA)
                                {
                                    a = _eList->erase(a);
                                    has_been_del = true;
                                }
                                if (delB)
                                {
                                    if (a == b)
                                        a = _eList->erase(b);
                                    else
                                        _eList->erase(b);
                                }
                                break ;
                            }
                        }
                    }
                    if (!has_been_del)
                        ++a;
                }
            }
        }
        virtual IPacket                 *out(EventSum &) { return 0; }
        virtual void                    in(IPacket *p)
        {
            TcpPacket                   *packet;

            if ((packet = dynamic_cast<TcpPacket*>(p))
                    && p->getQuery() == static_cast<uint16_t>(Codes::JsonHitboxes))
            {
                std::string tmp =std::string(static_cast<const char *>(p->getData()), p->getSize());
                Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("hitboxes");
                for (auto &x : e.manager.getAll<Entity>())
                {
                    _hitboxes[x.first] = std::pair<int, int>(
                            x.second.manager.get<int>("x"),
                            x.second.manager.get<int>("y"));
                }
            }
        }

        virtual bool                    handle(EventSum e) {
            if (e == E_Stage)
                _isActiv = !_isActiv;
            return false;
        }

        virtual std::vector<REvent>     &broadcast(void) { return _eventList; }

        virtual EventSum                getEvent()
        {
            EventSum          tmp = 0;

            if (_event != noEvent)
            {
                tmp = _event;
                _event = noEvent;
            }
            return tmp;
        }
    private:
        bool	                                                    _isActiv;
        std::list<Entity*>                                          *_eList;
        std::unordered_map<std::string, std::pair<int, int> >       _hitboxes;
        EventSum														_event;

};

#endif
