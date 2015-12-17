#ifndef MOVEMENT_HH_
# define MOVEMENT_HH_

#include "System/ASystem.hh"
#include "System/Shoot/Pattern.hh"
#include "Network/UdpSocket.hh"

class MovementSystem : public ASystem
{
    public:
        MovementSystem() : lastEvent(noEvent) {};
	MovementSystem(std::list<Entity*> *eList) : _eList(eList),
												lastEvent(noEvent), isActiv(false) {
            _eventList.push_back(Key_Up);
            _eventList.push_back(Key_Down);
            _eventList.push_back(Key_Left);
            _eventList.push_back(Key_Right);
			_eventList.push_back(E_Stage);
            _frequency= 0;
            _lastId = 0;
        }

        virtual ~MovementSystem() {};

        virtual void                    update(int duration)
        {
            _frequency+= duration;
			if (!isActiv)
				return ;
            float k = 1.0f;

            for (unsigned i = 1; i < sizeof(EventSum) * 8; i <<= 1)
            {
                if (lastEvent & i)
                    ++k;
                if (k >= 2)
                    break ;
            }

            float move = static_cast<float>(duration);
            for(auto x : *_eList)
            {
                if (x->manager.get<std::string>("name") == "player1")
                {
					move *= (x->manager.get<float>("velocity") / k);
                    std::pair<float, float> tmp =
                        x->manager.get<std::pair<float, float> >("position");
                    if (lastEvent & Key_Up)
                        tmp.second -= move;
                    if (lastEvent & Key_Down)
                        tmp.second += move;
                    if (lastEvent & Key_Left)
                        tmp.first -= move;
                    if (lastEvent & Key_Right)
                        tmp.first += move;
                    tmp.first = (tmp.first < 0 ? 0 : tmp.first);
                    tmp.first = (tmp.first > 1920 ? 1920 : tmp.first);
                    tmp.second = (tmp.second < 0 ? 0 : tmp.second);
                    tmp.second = (tmp.second > 1080 ? 1080 : tmp.second);
                    x->manager.set("position", tmp);
                    break;
                }
            }
			lastEvent = 0;
        }

        virtual IPacket                 *out() {
            if (isActiv == false)
                return (0);
            if (lastEvent && _frequency > 30)
            {
                std::string     tmp = std::to_string(lastEvent);
                _packet.setQuery(static_cast<uint16_t>(UdpCodes::KeyPressed));
                _packet.setData(tmp.c_str());
                _packet.setSize(static_cast<uint16_t>(tmp.size()));
                _frequency = 0;
                return (&_packet);
            }
            return (0);
        }

        virtual void                    in(IPacket *p){
            UdpPacket   *packet;

            if ((packet = dynamic_cast<UdpPacket*>(p)) &&
				packet->getQuery() == static_cast<uint16_t>(UdpCodes::NewPos))
            {
                std::string tmp = std::string(
                        static_cast<const char *>(packet->getData()), packet->getSize());
                float px, py;
                std::string name = tmp.substr(0, tmp.find(":")).c_str();
                tmp = tmp.substr(tmp.find(":") + 1);
                px = std::atof(tmp.substr(0, tmp.find(":")).c_str());
                py = std::atof(tmp.substr(tmp.find(":") + 1).c_str());
                if (_lastId < packet->getID())
                {
                    _lastId = packet->getID();
                    for(auto x : *_eList)
                    {
                        try {
                        if (x->manager.get<std::string>("pseudo") == name)
                        {
                            x->manager.set<std::pair<float, float> >
								("position", std::pair<float, float>(px, py));
                        }
                        } catch (ComponentManagerException &) {}
                    }
                }
            }
        }

        virtual bool                    handle(EventSum e)
        {
            if (e == E_Stage)
                isActiv = !isActiv;
            if (e & Key_Up || e & Key_Down || e & Key_Left || e & Key_Right
				|| e & Key_Fire || e & Key_Change)
			{
				if ((e & Key_Fire))
					e = e & ~Key_Fire;
                lastEvent = e;
			}
            return (true);
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
        std::list<Entity*>	*_eList;
        EventSum			lastEvent;
        bool				isActiv;
        UdpPacket           _packet;
        int                 _frequency;
        uint64_t            _lastId;
};

#endif // MOVEMENT_HH_
