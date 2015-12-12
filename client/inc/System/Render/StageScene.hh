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
            Scene(win), _stageNb(3), _direction(noEvent)
    {
        _entities.push_back(&_b1);
        _entities.push_back(&_b2);
        _entities.push_back(&_b3);
        _entities.push_back(&_gui);
        _entities.push_back(&_b4);

        _ship.load("client/res/ship/player-ship-grey2_111.png", true);

        for (int i = 1; i <= 3; ++i)
        {
            ScrollingSprite *s1 = new ScrollingSprite();
            ScrollingSprite *s2 = new ScrollingSprite();
            ScrollingSprite *s3 = new ScrollingSprite();
            ScrollingSprite *s4 = new ScrollingSprite();
            s1->load("client/res/stages/stage" + std::to_string(i) + "/background_7680.png", 1);
            s2->load("client/res/stages/stage" + std::to_string(i) + "/middle_7680.png", 2);
            s3->load("client/res/stages/stage" + std::to_string(i) + "/middle2_7680.png", 3);
            s4->load("client/res/stages/stage" + std::to_string(i) + "/top_7680.png", 4);
            _s1.push_back(s1);
            _s2.push_back(s2);
            _s3.push_back(s3);
            _s4.push_back(s4);
        }
        _b1.manager.add<AView*>("view", &_view);
        _b1.manager.add<ADrawable*>("background", _s1[_stageNb - 1]);
        _b2.manager.add<ADrawable*>("background", _s2[_stageNb - 1]);
        _b3.manager.add<ADrawable*>("background", _s3[_stageNb - 1]);
        _b4.manager.add<ADrawable*>("background", _s4[_stageNb - 1]);
        _gui.manager.add<ADrawable*>("ship", &_ship);
    }

        virtual ~StageScene()
        {
            _s1.erase(_s1.begin(), _s1.end());
            _s2.erase(_s2.begin(), _s2.end());
            _s3.erase(_s3.begin(), _s3.end());
            _s4.erase(_s4.begin(), _s4.end());
        }

        virtual void    handle(EventSum e, EventSum&)
        {
            if (e & Key_Up || e & Key_Down || e & Key_Left || e & Key_Right)
                _direction = e;
        }

        virtual void    update(int duration)
        {
            _win.setMenuMode(false);
			float move = duration * 0.75f;
            _s1[_stageNb - 1]->update(duration);
            _s2[_stageNb - 1]->update(duration);
            _s3[_stageNb - 1]->update(duration);
            _s4[_stageNb - 1]->update(duration);
            if (_direction & Key_Up)
                _ship.setPosition(_ship.getPosition() +
                        sf::Vector2f(0.0f, -move));
            if (_direction & Key_Down)
                _ship.setPosition(_ship.getPosition() +
                        sf::Vector2f(0.0f, move));
            if (_direction & Key_Left)
                _ship.setPosition(_ship.getPosition() +
                        sf::Vector2f(-move, 0.0f));
            if (_direction & Key_Right)
                _ship.setPosition(_ship.getPosition() +
                        sf::Vector2f(move, 0.0f));
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
        std::vector<ScrollingSprite*>   _s1;
        std::vector<ScrollingSprite*>   _s2;
        std::vector<ScrollingSprite*>   _s3;
        std::vector<ScrollingSprite*>   _s4;
        int                             _stageNb;
        AnimatedSprite                  _ship;

        EventSum         _direction;
};


#endif /* end of include guard: STAGESCENE_HH_0PY2FK7L */
