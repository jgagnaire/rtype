#ifndef GAMEROOMSCENE_HH_ZNKJPAAS
#define GAMEROOMSCENE_HH_ZNKJPAAS

#include <unordered_map>
#include "Network/NetworkManager.hh"
#include "Network/TcpSocket.hh"
#include "Scene.hh"
#include "System/Render/Text.hh"
#include "System/Render/View.hh"

class GameRoomScene : public Scene
{
    public:
        GameRoomScene(IWindow &win, std::unordered_map<uint64_t, Entity*> *e):
            Scene(win, e), _buttons(2), _update(true),
            _new(false), _current(0), _currentR(0),
            _event(0), _lastCode(Codes::nothing)
    {
        _buttons[0].setText("Join");
        _buttons[0].setPosition(500, 50);
        _buttons[1].setText("Create");
        _buttons[1].setPosition(1100, 50);

        _b1.manager.add<View*>("view", &_view);
        _texts.manager.add<ADrawable*>("buffer", &_bufferText);
        for (std::size_t i = 0; i < _buttons.size(); ++i)
            _texts.manager.add<ADrawable*>("button" + std::to_string(i),
                    &_buttons[i]);
    }

        virtual ~GameRoomScene()
        {}

        virtual void    update(int)
        {
            _win.setMenuMode(true);
            _win.draw(_b1);
            _win.draw(_texts);
            _win.draw(_changingText);
        }

        virtual void    handle(EventSum e, EventSum &send)
        {
            std::string text;

            if (_event && _lastCode != Codes::nothing)
            {
                send = _event;
                _event = 0;
                _lastCode = Codes::nothing;
                _currentR = 0;
                _current = 0;
                return ;
            }
            if (e & Key_Change && e != Key_Change)
            {
                EventSum tmp = (e << 1) >> 1;

                if (tmp == 125)
                {
                    _update = true;
                    return ;
                }
                if (tmp == 127 && _buffer.empty() == false)
                    _buffer.erase(_buffer.size() - 1, 1);
                if (tmp < 125)
                    _buffer += static_cast<char>(tmp);
                _bufferText.setText(_buffer);
                _bufferText.setCenter();
                if (tmp == 126)
                {
                    uint64_t i = 0;
                    switch (_current)
                    {
                        case 0:
                            if (_rooms.empty() == false)
                            {
                                _packet.setQuery(static_cast<uint16_t>(Codes::JoinRoom));
                                for (auto x : _rooms)
                                    if (_currentR == i++)
                                        _name = x.first;
                                _packet.setData(_name.c_str());
                                _packet.setSize(static_cast<uint16_t>(_name.size()));
                                _new = true;
                            }
                            break;
                        case 1:
                            _packet.setQuery(static_cast<uint16_t>(Codes::CreateRoom));
                            _packet.setData(_buffer.c_str());
                            _packet.setSize(static_cast<uint16_t>(_buffer.size()));
                            _new = true;
                            break;
                    }
                }
                return ;
            }
            if (e & Key_Left && _current > 0)
                --_current;
            else if (e & Key_Right && _current < _buttons.size() - 1)
                ++_current;
            if (e & Key_Up && _currentR > 0)
                --_currentR;
            else if (e & Key_Down && _currentR < _rooms.size() - 1)
                ++_currentR;
            for (std::size_t i = 0; i <  _buttons.size(); ++i)
                _buttons[i].setColor(0xffffffff);
            _buttons[static_cast<unsigned int>(_current)].setColor(0xff0000ff);
            uint64_t i = 0;
            for (auto x : _rooms)
            {
                if (i == _currentR)
                    x.second->setColor(0xff0000ff);
                else
                    x.second->setColor(0xffffffff);
                ++i;
            }
        }

        virtual void    in(IPacket *p, std::string&)
        {
            std::string tmp, name, nb;
            TcpPacket   *packet;
            uint64_t i = 0;

            if ((packet = dynamic_cast<TcpPacket*>(p)))
            {
                switch (static_cast<Codes>(packet->getQuery()))
                {
                    case Codes::Ok:
                        if (_lastCode != Codes::nothing)
                            _event = E_Ready;
                        break;
                    case Codes::AlreadyInRoom:
                    case Codes::NotLoggedIn:
                        std::cerr << "PROBLEM IN ROOMS" << std::endl;
                        break;
                    case Codes::ExistingRoom:
                        tmp = static_cast<const char*>(packet->getData());
                        name = tmp.substr(0, tmp.find(":"));
                        nb = tmp.substr(tmp.find(":") + 1, 1);
                        _rooms[name] = new Text(name + "    " + nb + "/4");
                        _rooms[name]->setCenter();
                        _changingText.manager.add<ADrawable*>(name, _rooms[name]);
                        _currentR = 0;
                        for (auto x : _rooms)
                            x.second->setY(++i * 75 + 50);
                    default:
                        ;
                }
            }
        }

        virtual IPacket *out(EventSum&)
        {
            if (_new)
            {
                _new = false;
                _lastCode = static_cast<Codes>(_packet.getQuery());
                return (&_packet);
            }
            if (_update)
            {
                _packet.setQuery(static_cast<uint16_t>(Codes::GetGameRooms));
                _packet.setData(0);
                _packet.setSize(0);
                _update = false;
                _rooms.clear();
                _changingText.manager.removeAll();
                _currentR = 0;
                return (&_packet);
            }
            return (0);
        }

    private:
        std::vector<Text>                                   _buttons;
        std::unordered_map<std::string, Text*>              _rooms;
        View                                                _view;
        Entity                                              _b1;
        Entity                                              _texts;
        Entity                                              _changingText;
        TcpPacket                                           _packet;
        bool                                                _update;
        bool                                                _new;
        std::string                                         _buffer;
        Text                                                _bufferText;
        uint64_t                                         _current;
        uint64_t                                         _currentR;
        EventSum                                            _event;
        Codes                                               _lastCode;
        std::string                                         _name;
};

#endif /* end of include guard: GAMEROOMSCENE_HH_ZNKJPAAS */
