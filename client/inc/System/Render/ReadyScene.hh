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
            Scene(win, e), _isReady(false), _send(false)
    {
        _b1.manager.add<View*>("view", &_view);
        _texts.manager.add<ADrawable*>("isReady", &_isReadyText);
        _texts.manager.add<ADrawable*>("list", &_playersText);
    }

        virtual ~ReadyScene()
        {}

        virtual void    update(int)
        {
            _win.setMenuMode(true);
            _win.draw(_b1);
            _win.draw(_texts);
        }

        virtual void    handle(EventSum e, EventSum&)
        {
            if (e != Key_Change && e & Key_Change)
            {
                EventSum tmp = (e << 1) >> 1;
                if (tmp == 125)
                {
                    if (_isReady)
                        _packet.setQuery(static_cast<uint16_t>(Codes::NotReady));
                    else
                        _packet.setQuery(static_cast<uint16_t>(Codes::Ready));
                }
                else if (tmp == 127)
                    _packet.setQuery(static_cast<uint16_t>(Codes::LeaveRoom));
            }
        }

        virtual void    in(IPacket *)
        {
        }

        virtual IPacket *out()
        {
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
};


#endif /* end of include guard: READYSCENE_HH_Q1STKA62 */
