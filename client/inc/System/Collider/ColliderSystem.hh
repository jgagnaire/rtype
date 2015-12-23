#ifndef COLLIDER_HH_
# define COLLIDER_HH_

#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"
#include "Utility/JSONParser.hh"
#include "Network/NetworkManager.hh"
#include "System/Collider/FCollision.hh"

class ColliderSystem : public ASystem
{
    public:
        ColliderSystem() {}
        ColliderSystem(std::unordered_map<uint64_t, Entity*> *list) :
            _isActiv(false), _eList(list) {
                _eventList.push_back(E_Stage);
                _event = noEvent;
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

        void        collide(uint64_t id1, uint64_t id2)
        {
            Entity          *a, *b;

            a = (*_eList)[id1];
            b = (*_eList)[id2];
            std::pair<float, float> ex(-1, -1);
            bool delA = a->manager.get<fCollision>("collision")(*a, *b, ex);
            if (ex.first > -1)
            {
                (*_eList)[(*_eList)[-1]->manager.get<uint64_t>("lastExplosion")] = createExplosion(ex);
                (*_eList)[-1]->manager.set<uint64_t>("lastExplosion", (*_eList)[-1]->manager.get<uint64_t>("lastExplosion") + 1);
            }
            ex.first = ex.second = -1;
            bool delB = b->manager.get<fCollision>("collision")(*b, *a, ex);
            if (ex.first > -1)
            {
                (*_eList)[(*_eList)[-1]->manager.get<uint64_t>("lastExplosion")] = createExplosion(ex);
                (*_eList)[-1]->manager.set<uint64_t>("lastExplosion", (*_eList)[-1]->manager.get<uint64_t>("lastExplosion") + 1);
            }
            if (delA)
                _eList->erase(id1);
            if (delB)
                _eList->erase(id2);
            std::cout << "COLLIDE "<< id1 << " -- " << id2 << std::endl;
        }

        virtual void                    update(int duration)
        {
            int time = 0;
            int time2 = 0;

            for (auto a : *_eList)
            {
                if (a.second->manager.get<std::string>("type") == "player" &&
                        ((time = a.second->manager.get<int>("perfect_shield")) > 0 ||
                         (time2 = a.second->manager.get<int>("respawn")) > 0))
                {
                    time -= duration;
                    time2 -= duration;
                    if (time <= 0)
                        time = 0;
                    if (time2 <= 0)
                        time2 = 0;
                    a.second->manager.set<int>("perfect_shield", time);
                    a.second->manager.set<int>("respawn", time2);
                }
            }
            for (std::size_t i = 0; i < _untreated.size();)
            {
                std::cout << "ifExis= " << _untreated[i].first << " = " << _untreated[i].second << std::endl;
                std::cout << "ifExist " << (_eList->find(_untreated[i].first) != _eList->end()) <<" = " <<
                    (_eList->find(_untreated[i].second) != _eList->end()) << std::endl;
                if (_eList->find(_untreated[i].first) != _eList->end()
                        && _eList->find(_untreated[i].second) != _eList->end())
                {
                    std::cout << "retest "<< _untreated[i].first << " -- " << _untreated[i].second << std::endl;
                    collide(_untreated[i].first, _untreated[i].second);
                    _untreated.erase(_untreated.begin() + i);
                    --i;
                }
                ++i;
            }
        }

        virtual IPacket                 *out(EventSum &) { return 0; }

        virtual void                    in(IPacket *p)
        {
            TcpPacket                   *packet;
            UdpPacket                   *up;

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
            if ((up = dynamic_cast<UdpPacket*>(p))
                    && p->getQuery() == static_cast<uint16_t>(UdpCodes::Collided))
            {
                std::string data = std::string(
                        static_cast<const char*>(up->getData()),
                        up->getSize());
                uint64_t     id1, id2;

                id1 = std::stoll(data.substr(0, data.find(":")));
                id2 = std::stoll(data.substr(data.find(":") + 1));
                std::cout << id1 << " -- " << id2 << std::endl;
                std::cout << "NOP" << (_eList->find(id1) != _eList->end()) <<" = " <<
                    (_eList->find(id2) != _eList->end()) << std::endl;
                if (_eList->find(id1) == _eList->end()
                        || _eList->find(id2) == _eList->end())
                {
                    _untreated.push_back(std::pair<uint64_t, uint64_t>(id1, id2));
                    return ;
                }
                collide(id1, id2);
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
        std::unordered_map<uint64_t, Entity*>                    *_eList;
        std::unordered_map<std::string, std::pair<int, int> >       _hitboxes;
        EventSum														_event;
        std::vector<std::pair<uint64_t, uint64_t> >           _untreated;
};

#endif
