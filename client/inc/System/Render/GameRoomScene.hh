#ifndef GAMEROOMSCENE_HH_ZNKJPAAS
#define GAMEROOMSCENE_HH_ZNKJPAAS

#include "Scene.hh"

class GameRoomScene : public Scene
{
    public:
        GameRoomScene(IWindow &win):
            Scene(win)
    {}

        virtual ~GameRoomScene()
        {}

        virtual void    update(int)
        {
        }

        virtual void    handle(EventSum, EventSum&)
        {}
};

#endif /* end of include guard: GAMEROOMSCENE_HH_ZNKJPAAS */
