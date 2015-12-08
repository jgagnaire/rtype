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
            _entities.push_back(&_gui);
            _entities.push_back(&_b3);

            _b1.manager.add<AView*>("view", &_view);
            _b1.manager.add<ADrawable*>("background", &_s1);
            _b2.manager.add<ADrawable*>("background", &_s2);
            _b3.manager.add<ADrawable*>("background", &_s3);
            _gui.manager.add<ADrawable*>("ship", &_ship);

            _s1.load("client/res/stages/stage1/background_7680.png", 1);
            _s2.load("client/res/stages/stage1/middle_7680.png", 2);
            _s3.load("client/res/stages/stage1/top_7680.png", 3);
            _ship.load("client/res/ship/player-ship-blue_107.png", true);
        }

        virtual ~StageScene()
        {

        }

        virtual void    handle(REvent e, REvent&)
        {
            switch (e)
            {
                case Key_Up:
                case Key_Down:
                case Key_Left:
                case Key_Right:
                    _direction = e;
                default:
                    ;
            }
        }

        virtual void    update(int duration)
        {
            _win.setMenuMode(false);
            int move = duration * 0.75;
            _s1.update(duration);
            _s2.update(duration);
            _s3.update(duration);
            switch (_direction)
            {
                case Key_Up:
                    _ship.setPosition(_ship.getPosition() +
                            sf::Vector2f(0, -move));
                    break;
                case Key_Down:
                    _ship.setPosition(_ship.getPosition() +
                            sf::Vector2f(0, move));
                    break;
                case Key_Left:
                    _ship.setPosition(_ship.getPosition() +
                            sf::Vector2f(-move, 0));
                    break;
                case Key_Right:
                    _ship.setPosition(_ship.getPosition() +
                            sf::Vector2f(move, 0));
                    break;
                default:
                    ;
            }
            if (_direction != noEvent)
                _direction = noEvent;
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

        REvent          _direction;
};


#endif /* end of include guard: STAGESCENE_HH_0PY2FK7L */
