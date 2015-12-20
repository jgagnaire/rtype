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

        virtual void                    update(int)
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
					d1 = (*a)->manager.get<Pattern::Side>("direction");
                    setSize(s1, (*a)->manager.get<std::string>("name"));
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
                                bool hasPlayer = (*a)->manager.get<std::string>("type") == "player"
                                    || (*b)->manager.get<std::string>("type") == "player";
                                bool hasBonus = (*a)->manager.get<std::string>("type") == "bonus"
                                    || (*b)->manager.get<std::string>("type") == "bonus";
                                bool hasShoot = (*a)->manager.get<std::string>("type") == "shoot"
                                    || (*b)->manager.get<std::string>("type") == "shoot";
                                if (!((hasPlayer && hasBonus) || (hasBonus && hasShoot)))
                                {
                                    _eList->push_back(createExplosion(p2));
									_event = E_Explosion;
                                    a = _eList->erase(a);
                                    has_been_del = true;
                                }
                                if (!(hasShoot && hasBonus))
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
