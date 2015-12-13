#ifndef READYSCENE_HH_Q1STKA62
#define READYSCENE_HH_Q1STKA62

#include "Scene.hh"
#include "Network/TcpSocket.hh"
#include "System/Render/Text.hh"

class GameRoomScene : public Scene
{
    public:
        GameRoomScene(IWindow &win, std::list<Entity*> *e):
            Scene(win, e)
    {
    }

        virtual ~GameRoomScene()
        {}

        virtual void    update(int)
        {
            _win.setMenuMode(true);
            _win.draw(_b1);
            _win.draw(_texts);
        }

        virtual void    handle(EventSum e, EventSum&)
        {
       }

        virtual void    in(IPacket *p)
        {
       }

        virtual IPacket *out()
        {
       }

    private:
        View                                                _view;
        Entity                                              _b1;
        Entity                                              _texts;
        TcpPacket                                           _packet;
};


#endif /* end of include guard: READYSCENE_HH_Q1STKA62 */
