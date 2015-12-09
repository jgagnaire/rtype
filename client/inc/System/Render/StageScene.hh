#ifndef STAGESCENE_HH_0PY2FK7L
# define STAGESCENE_HH_0PY2FK7L

# include "System/Render/Scene.hh"
# include "System/Render/ScrollingSprite.hh"
# include "System/Render/AnimatedSprite.hh"
# include "System/Render/View.hh"

class StageScene : public Scene
{
    public:
        StageScene(IWindow &win):
            Scene(win), _direction(noEvent)
        {
            _entities.push_back(&_b1);
            _entities.push_back(&_b2);
            _entities.push_back(&_b3);
            _entities.push_back(&_gui);
            _entities.push_back(&_b4);

            _b1.manager.add<AView*>("view", &_view);
            _b1.manager.add<ADrawable*>("background", &_s1);
            _b2.manager.add<ADrawable*>("background", &_s2);
            _b3.manager.add<ADrawable*>("background", &_s3);
            _b4.manager.add<ADrawable*>("background", &_s4);
            _gui.manager.add<ADrawable*>("ship", &_ship);

            _s1.load("client/res/stages/stage2/background_7680.png", 1);
            _s2.load("client/res/stages/stage2/middle_7680.png", 2);
            _s3.load("client/res/stages/stage2/middle2_7680.png", 3);
            _s4.load("client/res/stages/stage2/top_7680.png", 4);
            _ship.load("client/res/ship/player-ship-blue_107.png", true);
        }

        virtual ~StageScene()
        {

        }

        virtual void    handle(REvents e, REvents&)
        {
            if (e & Key_Up || e & Key_Down || e & Key_Left || e & Key_Right)
                _direction = e;
        }

        virtual void    update(int duration)
        {
            _win.setMenuMode(false);
            int move = duration * 0.75;
            _s1.update(duration);
            _s2.update(duration);
            _s3.update(duration);
            _s4.update(duration);
            if (_direction & Key_Up)
                _ship.setPosition(_ship.getPosition() +
                        sf::Vector2f(0, -move));
            if (_direction & Key_Down)
            _ship.setPosition(_ship.getPosition() +
                    sf::Vector2f(0, move));
            if (_direction & Key_Left)
            _ship.setPosition(_ship.getPosition() +
                    sf::Vector2f(-move, 0));
            if (_direction & Key_Right)
            _ship.setPosition(_ship.getPosition() +
                    sf::Vector2f(move, 0));
            if (_direction != 0)
                _direction = 0;
            _ship.update(duration);
        }

    private:
        Entity          _b1;
        Entity          _b2;
        Entity          _b3;
        Entity          _b4;
        Entity          _gui;

        View            _view;
        ScrollingSprite _s1;
        ScrollingSprite _s2;
        ScrollingSprite _s3;
        ScrollingSprite _s4;
        AnimatedSprite  _ship;

        REvents         _direction;
};


#endif /* end of include guard: STAGESCENE_HH_0PY2FK7L */
