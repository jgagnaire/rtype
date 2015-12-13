#ifndef STAGESCENE_HH_0PY2FK7L
# define STAGESCENE_HH_0PY2FK7L

# include "System/Render/Scene.hh"
# include "System/Render/ScrollingSprite.hh"
# include "System/Render/AnimatedSprite.hh"
# include "System/Render/View.hh"

class StageScene : public Scene
{
    public:
        StageScene(IWindow &win, std::list<Entity*> *e):
            Scene(win, e), _stageNb(3), _direction(noEvent)
    {
        _ship.load("client/res/ship/player-ship-grey2_111.png", true);
        _shoot.load("client/res/bullet.png", true);

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
        _guiPlayers.manager.add<ADrawable*>("ship", &_ship);
        _guiShoots.manager.add<ADrawable*>("shoot", &_shoot);
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
            _s1[_stageNb - 1]->update(duration);
            _s2[_stageNb - 1]->update(duration);
            _s3[_stageNb - 1]->update(duration);
            _s4[_stageNb - 1]->update(duration);
            _win.draw(_b1);
            _win.draw(_b2);
            _win.draw(_b3);
            std::cout << "Entites " << _entities->size() << std::endl;
            for (auto x : *_entities)
            {
                if (x->manager.get<std::string>("type") == "shoot")
                {
					std::cout << "Shoot : " << x->manager.get<std::pair<float, float> >("position").first << " - " << x->manager.get<std::pair<float, float> >("position").second << std::endl;
					
                    _shoot.setPosition(sf::Vector2f(x->manager.get<std::pair<float, float> >("position").first,
                                x->manager.get<std::pair<float, float> >("position").second));
                    _shoot.update(duration);
                    _win.draw(_guiShoots);
                }
                else if (x->manager.get<std::string>("type") == "player")
                {
                    _ship.setPosition(sf::Vector2f(x->manager.get<std::pair<float, float> >("position").first,
                                x->manager.get<std::pair<float, float> >("position").second));
                    _ship.update(duration);
                    _win.draw(_guiPlayers);
                }
            }
            _win.draw(_b4);
        }

    private:
        Entity          _b1;
        Entity          _b2;
        Entity          _b3;
        Entity          _b4;
        Entity          _guiPlayers;
        Entity          _guiShoots;

        View            _view;
        std::vector<ScrollingSprite*>   _s1;
        std::vector<ScrollingSprite*>   _s2;
        std::vector<ScrollingSprite*>   _s3;
        std::vector<ScrollingSprite*>   _s4;
        int                             _stageNb;
        AnimatedSprite                  _ship;
        AnimatedSprite                  _shoot;

        EventSum         _direction;
};


#endif /* end of include guard: STAGESCENE_HH_0PY2FK7L */
