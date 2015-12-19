#ifndef COLLIDER_HH_
# define COLLIDER_HH_

#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"

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
            return e;
        }

        void                            getSize(std::pair<int, int> &s,
                const std::string &type, const std::string &name)
        {
            if (type == "player")
            {
                s.first = 111;
                s.second = 39;
            }
            else if (type == "shoot")
            {
                s.first = 20;
                s.second = 12;
            }
            else if (type == "mob")
            {
                if (name == "mob1")
                {
                    s.first = 97;
                    s.second = 72;
                }
                else if (name == "mob2")
                {
                    s.first = 114;
                    s.second = 39;
                }
                else if (name == "mob3")
                {
                    s.first = 58;
                    s.second = 87;
                }
            }
            else
            {
                s.first = 0;
                s.second = 0;
            }
        }

        virtual void                    update(int)
        {
            std::pair<float, float> p1, p2;
            std::pair<int, int>     s1, s2;
            std::string             t1, t2;
			Pattern::Side			d1, d2;

            if (_isActiv)
            {
                for (auto a = _eList->begin(); a != _eList->end(); ++a)
                {
                    p1 = (*a)->manager.get<std::pair<float, float> >("position");
                    t1 = (*a)->manager.get<std::string>("type");
					d1 = (*a)->manager.get<Pattern::Side>("direction");
                    getSize(s1, t1, (*a)->manager.get<std::string>("name"));
                    for (auto b = _eList->begin(); b != _eList->end(); ++b)
                    {
                        if (*a != *b)
                        {
                            p2 = (*b)->manager.get<std::pair<float, float> >("position");
                            t2 = (*b)->manager.get<std::string>("type");
							d2 = (*b)->manager.get<Pattern::Side>("direction");
                            getSize(s2, t2, (*b)->manager.get<std::string>("name"));
                            if (t1 != t2 && d1 != d2 && p1.first < p2.first + s2.first &&
                                    p1.first + s1.first > p2.first &&
                                    p1.second < p2.second + s2.second &&
                                    s1.second + p1.second > p2.second)
                            {
                                _eList->push_back(createExplosion(p2));
                                b = _eList->erase(b);
                                a = _eList->erase(a);
                                if (a != _eList->end())
                                    --a;
                                break ;
                            }
                        }
                    }
                }
            }
        }
        virtual IPacket                 *out(EventSum &) { return 0; }
        virtual void                    in(IPacket*) {}

        virtual bool                    handle(EventSum e) {
            if (e == E_Stage)
                _isActiv = !_isActiv;
            return false;
        }

        virtual std::vector<REvent>     &broadcast(void) { return _eventList; }

        virtual EventSum                getEvent() { return noEvent; }
    private:
        bool	                        _isActiv;
        std::list<Entity*>              *_eList;

};

#endif
