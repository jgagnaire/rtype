#ifndef COLLIDER_HH_
# define COLLIDER_HH_

#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"
#include "Utility/JSONParser.hh"

class ColliderSystem : public ASystem
{
    public:
        ColliderSystem() {}
        ColliderSystem(std::unordered_map<std::size_t, Entity*> *list) :
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

        virtual void                    update(int)
        {
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
        std::unordered_map<std::size_t, Entity*>                    *_eList;
        std::unordered_map<std::string, std::pair<int, int> >       _hitboxes;
        EventSum														_event;

};

#endif
