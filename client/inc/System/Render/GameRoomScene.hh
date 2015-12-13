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
        GameRoomScene(IWindow &win, std::list<Entity*> *e):
            Scene(win, e), _buttons(3), _update(true),
            _new(false), _current(0), _currentR(0),
            _event(0), _lastCode(0)
    {
        _buttons[0].setText("Random");
        _buttons[0].setPosition(300, 50);
        _buttons[1].setText("Join");
        _buttons[1].setCenter();
        _buttons[1].setY(50);
        _buttons[2].setText("Create");
        _buttons[2].setPosition(1300, 50);

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

            if (_event && _lastCode)
            {
                send = _event;
                _event = 0;
                _lastCode = 0;
                return ;
            }
            if (e == Key_Change)
            {
                _update = true;
                return ;
            }
            if (e & Key_Change && e != Key_Change)
            {
                EventSum tmp = (e << 1) >> 1;

                if (tmp == 127 && _buffer.empty() == false)
                    _buffer.erase(_buffer.size() - 1, 1);
                if (tmp < 125)
                    _buffer += static_cast<char>(tmp);
                _bufferText.setText(_buffer);
                _bufferText.setCenter();
                if (tmp == 126)
                {
                    std::string name;
                    std::size_t i = 0;
                    switch (_current)
                    {
                        case 0:
                            _packet.setQuery(static_cast<uint16_t>(Codes::RandomRoom));
                            break;
                        case 1:
                            _packet.setQuery(static_cast<uint16_t>(Codes::JoinRoom));
                            for (auto x : _rooms)
                                if (_currentR == i++)
                                    name = x.first;
                            _packet.setData(name.c_str());
                            _packet.setSize(static_cast<uint16_t>(name.size()));
                            break;
                        case 2:
                            _packet.setQuery(static_cast<uint16_t>(Codes::CreateRoom));
                            _packet.setData(_buffer.c_str());
                            _packet.setSize(static_cast<uint16_t>(_buffer.size()));
                            break;
                    }
                    _new = true;
                }
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
            _buttons[_current].setColor(0xff0000ff);
            std::size_t i = 0;
            for (auto x : _rooms)
            {
                if (i == _currentR)
                    x.second->setColor(0xff0000ff);
                else
                    x.second->setColor(0xffffffff);
                ++i;
            }
        }

        virtual void    in(IPacket *p)
        {
            std::string tmp, name, nb;
            TcpPacket   *packet;
            std::size_t i = 0;

            if ((packet = dynamic_cast<TcpPacket*>(p)))
            {
                switch (static_cast<Codes>(packet->getQuery()))
                {
                    case Codes::Ok:
                        if (_lastCode)
                            _event = E_Ready;
                        break;
                    case Codes::AlreadyInRoom:
                    case Codes::NotLoggedIn:
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

        virtual IPacket *out()
        {
            if (_new)
            {
                _new = false;
                _lastCode = _packet.getQuery();
                return (&_packet);
            }
            if (_update)
            {
                _packet.setQuery(static_cast<uint16_t>(Codes::GetGameRooms));
                _packet.setData(0);
                _packet.setSize(0);
                _update = false;
                _rooms.clear();
                for (auto x : _changingText.manager.getAll<ADrawable*>())
                    delete x;
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
        std::size_t                                         _current;
        std::size_t                                         _currentR;
        EventSum                                            _event;
        int                                                 _lastCode;
};

#endif /* end of include guard: GAMEROOMSCENE_HH_ZNKJPAAS */
