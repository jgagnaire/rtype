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
        ReadyScene(IWindow &win, std::list<Entity*> *e);

	virtual ~ReadyScene();

        virtual void    update(int);

        virtual void    handle(EventSum e, EventSum &send);

        virtual void    in(IPacket *p);

        virtual IPacket *out(EventSum&);
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
