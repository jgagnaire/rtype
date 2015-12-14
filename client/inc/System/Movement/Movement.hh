#ifndef MOVEMENT_HH_
# define MOVEMENT_HH_

#include "System/ASystem.hh"
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
        }

        virtual ~MovementSystem() {};

        virtual void                    update(int duration)
        {
			if (!isActiv)
				return ;
            float culpointastah = 1.0f;

            for (unsigned i = 1; i < sizeof(EventSum) * 8; i <<= 1)
            {
                if (lastEvent & i)
                    ++culpointastah;
                if (culpointastah >= 2)
                    break ;
            }

            float move = static_cast<float>(duration);
            for(auto x : *_eList)
            {
                if (x->manager.get<std::string>("name") == "player1")
                {
					move *= (x->manager.get<float>("velocity") / culpointastah);
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
        }
        virtual IPacket                 *out() {
            if (isActiv == false)
                return (0);
            if (lastEvent)
            {
                for(auto x : *_eList)
                if (x->manager.get<std::string>("name") == "player1")
                {
                    auto tmp = x->manager.get<std::pair<float, float> >("position");
                std::cout << tmp.first << "/" << tmp.second << std::endl;
                }
                std::string     tmp = std::to_string(lastEvent);
                _packet.setQuery(static_cast<uint16_t>(UdpCodes::KeyPressed));
                _packet.setData(tmp.c_str());
                _packet.setSize(static_cast<uint16_t>(tmp.size()));
                lastEvent = 0;
                return (&_packet);
            }
            return (0);
        }
        virtual void                    in(IPacket*){}
        virtual bool                    handle(EventSum e)
        {
            if (e == E_Stage)
                isActiv = !isActiv;
            if (e & Key_Up || e & Key_Down || e & Key_Left || e & Key_Right)
                lastEvent = e;
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
};

#endif // MOVEMENT_HH_
