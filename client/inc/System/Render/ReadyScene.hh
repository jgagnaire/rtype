#ifndef READYSCENE_HH_Q1STKA62
#define READYSCENE_HH_Q1STKA62

#include "Scene.hh"
#include "Network/NetworkManager.hh"
#include "Network/TcpSocket.hh"
#include "System/Render/Text.hh"
#include "System/Render/View.hh"

class ReadyScene : public Scene
{
    public:
        ReadyScene(IWindow &win, std::list<Entity*> *e):
            Scene(win, e), _isReady(false), _send(false),
            _new(false), _quit(false), _lastCode(0),
            _event(0)
    {
        _b1.manager.add<View*>("view", &_view);
        _texts.manager.add<ADrawable*>("isReady", &_isReadyText);
        _texts.manager.add<ADrawable*>("list", &_playersText);

        _isReadyText.setText("You are not ready.");
    }

        virtual ~ReadyScene()
        {}

        virtual void    update(int)
        {
            _win.setMenuMode(true);
            _win.draw(_b1);
            _win.draw(_texts);
        }

        virtual void    handle(EventSum e, EventSum &send)
        {
            if (_event)
            {
                send = _event;
                _event = 0;
                return ;
            }
            if (_quit)
            {
                send = E_GameRoom;
                _quit = false;
                _isReady = false;
                _isReadyText.setText("You are not ready.");
                return ;
            }
            if (e != Key_Change && e & Key_Change)
            {
                EventSum tmp = (e << 1) >> 1;
                if (tmp == 126)
                {
                    if (_isReady)
                        _packet.setQuery(static_cast<uint16_t>(Codes::NotReady));
                    else
                        _packet.setQuery(static_cast<uint16_t>(Codes::Ready));
                    _new = true;
                }
                else if (tmp == 127)
                {
                    _packet.setQuery(static_cast<uint16_t>(Codes::LeaveRoom));
                    _new = true;
                }
            }
        }

        virtual void    in(IPacket *p)
        {
            TcpPacket   *packet;
            std::string name;
            std::string tmp;
            Entity *e;

            if ((packet = dynamic_cast<TcpPacket*>(p)))
            {
                if (packet->getSize())
                    name = std::string(static_cast<const char*>(packet->getData()),
                            packet->getSize());
                switch (static_cast<Codes>(packet->getQuery()))
                {
                    case Codes::Ok:
                        if (static_cast<Codes>(_lastCode) == Codes::Ready
                                || static_cast<Codes>(_lastCode) == Codes::NotReady)
                            _isReady = !_isReady;
                        if (_isReady)
                            _isReadyText.setText("You are ready");
                        else
                            _isReadyText.setText("You are not ready.");
                        if (static_cast<Codes>(_lastCode) == Codes::LeaveRoom)
                        {
                            _quit = true;
                        }
                        break ;
                    case Codes::Begin:
                        e = new Entity();
                        for (auto x : _players)
                            e->manager.add<std::string>(x.first, x.first);
                        e->manager.add<std::string>("type", "playersData");
                        e->manager.add<std::string>("name", "playersData");
                        _entities->push_back(e);
                        _event = E_Stage;
                        break ;
                    case Codes::PlayerJoined:
                        _players[name] = true;
                        for (auto x : _players)
                            tmp += x.first + "\n";
                        _playersText.setText(tmp);
                        break ;
                    case Codes::PlayerLeft:
                        _players.erase(name);
                        for (auto x : _players)
                            tmp += x.first + "\n";
                        _playersText.setText(tmp);
                        break;
                    default:
                        ;
                }
            }
        }

        virtual IPacket *out()
        {
            if (_new)
            {
                _lastCode = _packet.getQuery();
                _new = false;
                return (&_packet);
            }
            return (0);
        }
    private:
        View                                                _view;
        Entity                                              _b1;
        Entity                                              _texts;
        TcpPacket                                           _packet;
        Text                                                _isReadyText;
        bool                                                _isReady;
        std::unordered_map<std::string, bool>               _players;
        Text                                                _playersText;
        bool                                                _send;
        bool                                                _new;
        bool                                                _quit;
        int                                                 _lastCode;
        EventSum                                            _event;
};


#endif /* end of include guard: READYSCENE_HH_Q1STKA62 */
