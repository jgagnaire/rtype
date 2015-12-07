#ifndef STAGESCENE_HH_0PY2FK7L
# define STAGESCENE_HH_0PY2FK7L

# include "System/Render/Scene.hh"
# include "System/Render/ScrollingSprite.hh"
# include "System/Render/View.hh"

class StageScene : public Scene
{
    public:
        StageScene()
        {
            _entities.push_back(&_b1);
            _entities.push_back(&_b2);
            _entities.push_back(&_b3);
            _entities.push_back(&_gui);

            _b1.manager.add("view", &_view);
            _b1.manager.add("background", &_s1);
            _b2.manager.add("background", &_s2);
            _b3.manager.add("background", &_s3);
            _gui.manager.add("ship", &_ship);

            _s1.load("client/res/stages/stage1/background_7680.png", 1);
            _s2.load("client/res/stages/stage1/middle_7680.png", 2);
            _s3.load("client/res/stages/stage1/top_7680.png", 3);
            _ship.load("client/res/ship/player-ship-blue_107.png", true);
        }

        virtual ~StageScene()
        {

        }

        virtual void    handle(REvent)
        {

        }

        virtual void    update(int duration)
        {
            _s1.update(duration);
            _s2.update(duration);
            _s3.update(duration);
            _ship.update(duration);
        }

    private:
        Entity          _b1;
        Entity          _b2;
        Entity          _b3;
        Entity          _gui;

        View            _view;
        ScrollingSprite _s1;
        ScrollingSprite _s2;
        ScrollingSprite _s3;
        AnimatedSprite  _ship;
};


#endif /* end of include guard: STAGESCENE_HH_0PY2FK7L */
