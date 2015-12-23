#ifndef SHOOTSYSTEM_HH_
# define SHOOTSYSTEM_HH_

# include "System/ASystem.hh"
# include "Pattern.hh"
# include "System/Collider/FCollision.hh"
# include "Network/UdpSocket.hh"
# include "Network/TcpSocket.hh"
# include "Network/NetworkManager.hh"
# include "Utility/JSONParser.hh"

class ShootSystem : public ASystem
{
    private:
        Entity	*createShoot(std::pair<float, float> pos, Pattern::MovePattern pattern, Pattern::Side side)
        {
            Entity *e = new Entity;

            e->manager.add<std::string>("name", "playerShoot");
            e->manager.add<std::string>("type", "shoot");
            e->manager.add("velocity", 4.50f);
            //TODO by json
            e->manager.add<int>("damage", 5);
            e->manager.add("position",
                    std::pair<float, float>(pos.first + 105.0f, pos.second + 9.0f));
            e->manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                ("pattern", patterns[pattern]);
            e->manager.add<Pattern::Side>("direction", side);
            if (side == Pattern::Side::RIGHT)
                e->manager.add<fCollision>("collision", Collision::shoot);
            else
                e->manager.add<fCollision>("collision", Collision::mobShoot);
            return (e);
        }
    public:
        ShootSystem(std::unordered_map<uint64_t, Entity*> *_list) :
            _eList(_list), fireRate(250), isActiv(false), _frequency(0), lastEvent(0)
    {
        _eventList.push_back(Key_Fire);
        _eventList.push_back(Key_Charge);
        _eventList.push_back(Key_Change);
        _eventList.push_back(E_Stage);
        _eventList.push_back(E_Ready);
        _eventList.push_back(NewStage);
        patterns[0] = Pattern::line;
        patterns[1] = Pattern::sinusoid;
        _durationAnimation = 0;
    }
        virtual ~ShootSystem() {}

        virtual void                    update(int duration)
        {
            if (_durationAnimation > 0)
            {
                _durationAnimation -= duration;
                _durationAnimation = (_durationAnimation <= 0 ? 0 : _durationAnimation);
                return ;
            }
            _frequency += duration;
            if (!isActiv)
                return ;
			this->fireRate += duration;
			bool has_been_del = false;
            for (auto x = _eList->begin(); x != _eList->end();)
            {
                has_been_del = false;
                if ((*x).second->manager.get<std::string>("type") == "shoot")
                {
                    (*x).second->manager.get<std::function<void (Entity&, Pattern::Side, int)> >
                        ("pattern")(*((*x).second), (*x).second->manager.
                                    get<Pattern::Side>("direction"), duration);
                    std::pair<float, float> tmp = (*x).second->manager.
                        get<std::pair<float, float> >("position");
                    if (tmp.first > 1920 * 100 || tmp.first < 0)
                    {
                        x = _eList->erase(x);
                        has_been_del = true;
                    }
                }
                if (!has_been_del)
                    ++x;
            }
            lastEvent = 0;
        }

        virtual IPacket                 *out(EventSum &e) {
            if (isActiv == false)
                return (0);
			if (lastEvent && _frequency > 30)
            {
                e |= lastEvent;
                _frequency = 0;
				return (&_packet);
            }
            return 0;
        }
        virtual void                    in(IPacket *p) {
            UdpPacket   *packet;

            if (dynamic_cast<TcpPacket*>(p) &&
                    p->getQuery() == static_cast<uint16_t>(Codes::JsonShoots))
            {
                std::string tmp = std::string(static_cast<const char *>(p->getData()), p->getSize());
                Entity &e = JSONParser::parse(tmp)->getEntity().manager.get<Entity>("fires");
                for (auto &x : e.manager.getAll<Entity>())
                {
                    _jsonEntities[x.first] = x.second;
                    _jsonEntities[x.first].manager.add<std::string>("name", "mobShoot");
                    _jsonEntities[x.first].manager.add<std::string>("type", "mobshoot");
                    _jsonEntities[x.first].manager.add<fCollision>("collision", Collision::mobShoot);
                    _jsonEntities[x.first].manager.add<Pattern::Side>("direction", Pattern::Side::LEFT);
                    _jsonEntities[x.first].manager.add<std::function<void (Entity&, Pattern::Side, int)> >
                            ("pattern", Pattern::getPattern(x.first));
                }
            }
            if ((packet = dynamic_cast<UdpPacket*>(p)) &&
                    packet->getQuery() == static_cast<uint16_t>(UdpCodes::ServeKeyPressed))
            {
                std::string tmp = std::string(
                        static_cast<const char *>(packet->getData()), packet->getSize());
                std::string name = tmp.substr(0, tmp.find(":"));
                std::string code = tmp.substr(name.size() + 1);
                EventSum e = static_cast<EventSum>(std::atof(code.c_str()));
                if (e & Key_Fire)
                {
                    for (auto x : *_eList)
                    {
                        if (x.second->manager.get<std::string>("type") == "player")
                            if (x.second->manager.get<std::string>("pseudo") == name)
                            {
                                Entity *sht = this->createShoot(x.second->manager.get<std::pair<float, float> >("position"),
                                        x.second->manager.get<Pattern::MovePattern>("pattern"),
                                        Pattern::Side::RIGHT);
                                sht->manager.add<Entity*>("Shooter", x.second);
                                (*_eList)[(*_eList)[-1]->manager.get<uint64_t>("lastShoot")] = sht;
                                (*_eList)[-1]->manager.set<uint64_t>("lastShoot", (*_eList)[-1]->manager.get<uint64_t>("lastShoot") + 1);
                                return ;
                            }
                    }
                }
                else if (e & Key_Change)
                {
                    for (auto x : *_eList)
                    {
                        if (x.second->manager.get<std::string>("name") == name)
                        {
                            x.second->manager.set<Pattern::MovePattern>
                                ("pattern", Pattern::incremente
                                 (x.second->manager.get<Pattern::MovePattern>("pattern")));
                            return ;
                        }
                    }
                }
            }
        }
        virtual bool                    handle(EventSum ev)
        {
            if (ev == NewStage)
                _durationAnimation = (*_eList)[-1]->manager.get<int>("changeDuration");
            if (ev == E_Stage)
                isActiv = true;
            if (ev == E_Ready)
            {
                isActiv = false;
                _jsonEntities.clear();
            }
            if (ev & Key_Fire && this->fireRate >= 250 && isActiv)
            {
                lastEvent = ev;
                fireRate = 0;
            }
            else if (ev & Key_Change && isActiv)
            {
                lastEvent = ev;
                for(auto x : *_eList)
                    if (x.second->manager.get<std::string>("name") == "player1")
                    {
                        x.second->manager.set<Pattern::MovePattern>
                            ("pattern", Pattern::incremente
                             (x.second->manager.get<Pattern::MovePattern>("pattern")));
                        break ;
                    }
            }
            return true;
        }
        virtual std::vector<REvent>     &broadcast(void)
        {
            return (_eventList);
        }

        virtual EventSum                getEvent()
        {
            return (noEvent);
        }

    protected:
        std::unordered_map<uint64_t, Entity*>	    *_eList;
        int					                        fireRate;
        bool				                        isActiv;
        UdpPacket				                    _packet;
        int                                         _frequency;
        EventSum			                        lastEvent;
        std::function<void (Entity&, Pattern::Side, int)> patterns[2];
        std::string                                 _tmp;
        int                                         _durationAnimation;
        std::unordered_map<std::string, Entity>     _jsonEntities;
};

#endif //SHOOTSYSTEM_HH_
