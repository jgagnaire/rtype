#ifndef STAGESCENE_HH_0PY2FK7L
# define STAGESCENE_HH_0PY2FK7L

# include "System/Render/Scene.hh"
# include "System/Render/ScrollingSprite.hh"
# include "System/Render/AnimatedSprite.hh"
# include "System/Render/View.hh"
# include "Network/UdpSocket.hh"
# include "Network/NetworkManager.hh"
# include "System/Shoot/Pattern.hh"
# include "System/Render/Text.hh"

class StageScene : public Scene
{
    public:
        StageScene(IWindow &win, std::unordered_map<uint64_t, Entity*> *e):
            Scene(win, e), _stageNb(1), _pSprites(15), _direction(noEvent),
            _numstage(5)
    {
        std::vector<std::string>    stages;
        stages.push_back("city");
        stages.push_back("destroy");
        stages.push_back("nectar");
        stages.push_back("terror");
        stages.push_back("reborn");
        _pSprites[0].load("client/res/ship/player-ship-blue2_111.png");
        _pSprites[1].load("client/res/ship/player-ship-green2_111.png");
        _pSprites[2].load("client/res/ship/player-ship-red2_111.png");
        _pSprites[3].load("client/res/ship/player-ship-purple2_111.png");
        _pSprites[4].load("client/res/mobs/mob-1_97.png");
        _pSprites[5].load("client/res/mobs/mob-2_114.png");
        _pSprites[6].load("client/res/mobs/mob-3.png");
        _pSprites[7].load("client/res/bonuses/force_38.png");
        _pSprites[8].load("client/res/bonuses/classic-shield_160.png");
        _pSprites[9].load("client/res/bonuses/perfect-shield_87.png");
        _pSprites[10].load("client/res/boss/boss-1_327.png");
        _pSprites[11].load("client/res/boss/boss-2_643.png");
        _pSprites[12].load("client/res/boss/boss-3_300.png");
        _pSprites[13].load("client/res/boss/boss-4_400.png");
        _pSprites[14].load("client/res/boss/boss-5_535.png");
        _shoot.load("client/res/bullet.png");
        _shootEnnemy.load("client/res/bullet2.png");
        //TODO, animation does not work _transition.load("client/res/transition_1920.png", false, 5);
        //_transition.setRepeat(false);
        _hud.load("client/res/HUD.png");
        for (auto &x : stages)
        {
            ScrollingSprite *s1 = new ScrollingSprite();
            ScrollingSprite *s2 = new ScrollingSprite();
            ScrollingSprite *s3 = new ScrollingSprite();
            ScrollingSprite *s4 = new ScrollingSprite();
            ScrollingSprite *s5 = new ScrollingSprite();
            s1->load("client/res/stages/" + x + "/background_7680.png", 1);
            s2->load("client/res/stages/" + x + "/middle_7680.png", 2);
            s3->load("client/res/stages/" + x + "/middle2_7680.png", 3);
            s4->load("client/res/stages/" + x + "/top2_7680.png", 4);
            s5->load("client/res/stages/" + x + "/top_7680.png", 4);
            _s1.push_back(s1);
            _s2.push_back(s2);
            _s3.push_back(s3);
            _s4.push_back(s4);
            _s5.push_back(s5);
        }
        _b1.manager.add<AView*>("view", &_view);
        _guiPlayers.manager.add<ADrawable*>("player", &(_pSprites[0]));
        _guiShoots.manager.add<ADrawable*>("shoot", &_shoot);
        _guiShootsEnnemy.manager.add<ADrawable*>("shoot", &_shootEnnemy);
        _lastId = 0;
        _b1.manager.add<ADrawable*>("background", _s1[_stageNb - 1]);
        _b2.manager.add<ADrawable*>("background", _s2[_stageNb - 1]);
        _b3.manager.add<ADrawable*>("background", _s3[_stageNb - 1]);
        _b4.manager.add<ADrawable*>("background", _s4[_stageNb - 1]);
        _b5.manager.add<ADrawable*>("background", _s5[_stageNb - 1]);
        _numstage[0].load("client/res/stages/numero-1_75.png", true);
        _numstage[1].load("client/res/stages/numero-2_115.png", true);
        _numstage[2].load("client/res/stages/numero-3_115.png", true);
        _numstage[3].load("client/res/stages/numero-4_230.png", true);
        _numstage[4].load("client/res/stages/numero-5_120.png", true);
        _stage.load("client/res/stages/stage_576.png", true);
        _stage.setPosition(sf::Vector2f(960 - 576 / 2 - 100, 540 - 123 / 2));
        for (auto &x : _numstage)
            x.setPosition(sf::Vector2f(960 - 576 / 2 + 576, 540 - 123 / 2));
        _changeScene.manager.add<ADrawable*>("2stage", &_stage);
        _changeScene.manager.add<ADrawable*>("2numero", &(_numstage[0]));
        _changeScene.manager.add<ADrawable*>("1", &_transition);
        _guiMobs.manager.add<ADrawable*>("sprite", &(_pSprites[4]));
        _guiExplosion.manager.add<ADrawable*>("explosion", 0);
        _b6.manager.add<ADrawable*>("hud", &_hud);
        _b7.manager.add<ADrawable*>("hudText", &_hudText);
    }

        void            switchStage()
        {
            _durationAnimation = (*_entities)[-1]->manager.get<int>("changeDuration");
            _changing = true;
        }

        void            setStage()
        {
            _b1.manager.set<ADrawable*>("background", _s1[_stageNb - 1]);
            _b2.manager.set<ADrawable*>("background", _s2[_stageNb - 1]);
            _b3.manager.set<ADrawable*>("background", _s3[_stageNb - 1]);
            _b4.manager.set<ADrawable*>("background", _s4[_stageNb - 1]);
            _b5.manager.set<ADrawable*>("background", _s5[_stageNb - 1]);
            _changeScene.manager.set<ADrawable*>("2numero", &(_numstage[_stageNb - 1]));
            _changeScene.manager.set<ADrawable*>("1", &_transition);
            (*_entities)[-1]->manager.set<uint64_t>("lastShoot", 1000000000);
            (*_entities)[-1]->manager.set<uint64_t>("lastMob", 2000000000);
            (*_entities)[-1]->manager.set<uint64_t>("lastBonus", 3000000000);
            (*_entities)[-1]->manager.set<uint64_t>("lastMobShoot", 4000000000);
            (*_entities)[-1]->manager.set<uint64_t>("lastBoss", 5000000000);
        }

        virtual void    init()
        {
            std::string     name;
            std::string     me;

            int i = -1;
            for (auto x : *_entities)
            {
                if (x.second->manager.get<std::string>("type") == "player")
                {
                    name = x.second->manager.get<std::string>("pseudo");
                    _players[name] = &(_pSprites[++i]);
                    _guiPlayers.manager.add<ADrawable*>(name, &(_pSprites[i]));
                }
            }
        }

        virtual ~StageScene()
        {
            _s1.erase(_s1.begin(), _s1.end());
            _s2.erase(_s2.begin(), _s2.end());
            _s3.erase(_s3.begin(), _s3.end());
            _s4.erase(_s4.begin(), _s4.end());
            _s5.erase(_s5.begin(), _s5.end());
        }

        virtual void    handle(EventSum e, EventSum&)
        {
            if (e == NewStage)
                switchStage();
            if (e & Key_Up || e & Key_Down || e & Key_Left || e & Key_Right)
                _direction = e;
        }

        virtual void    update(int duration)
        {
            _win.setMenuMode(false);
            if (_durationAnimation > 0)
            {
                if (_durationAnimation < 2500 && _changing)
                {
                    _changing = false;
                    ++_stageNb;
                    setStage();
                }
                _durationAnimation -= duration;
                if (_durationAnimation < 0)
                {
                    _durationAnimation = 0;
                }
                for (auto x : _changeScene.manager.getAll<ADrawable*>())
                    x.second->update(duration);
                if (_durationAnimation > 0)
                    _win.draw(_changeScene);
                return ;
            }
            _s1[_stageNb - 1]->update(duration);
            _s2[_stageNb - 1]->update(duration);
            _s3[_stageNb - 1]->update(duration);
            _s4[_stageNb - 1]->update(duration);
            _s5[_stageNb - 1]->update(duration);
            _win.draw(_b1);
            _win.draw(_b2);
            _win.draw(_b3);
            _win.draw(_b4);
            bool    has_been_del;
            for (auto x = _entities->begin(); x != _entities->end();)
            {
                has_been_del = false;
                if ((*x).second->manager.get<std::string>("type") == "shoot")
                {
                    _shoot.setPosition(sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first,
                                (*x).second->manager.get<std::pair<float, float> >("position").second));
                    if ((*x).second->manager.get<Pattern::Side>("direction") == Pattern::Side::RIGHT)
                    {
                        _shoot.update(duration);
                        _win.draw(_guiShoots);
                    }
                    else
                    {
                        _shootEnnemy.update(duration);
                        _win.draw(_guiShootsEnnemy);
                    }
                }
                else if ((*x).second->manager.get<std::string>("type") == "boss")
                {
                    if ((*x).second->manager.get<std::string>("name") == "boss1")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[10]));
                    else if ((*x).second->manager.get<std::string>("name") == "boss2")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[11]));
                    else if ((*x).second->manager.get<std::string>("name") == "boss3")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[12]));
                    else if ((*x).second->manager.get<std::string>("name") == "boss4")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[13]));
                    else if ((*x).second->manager.get<std::string>("name") == "boss5")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[14]));
                    static_cast<AnimatedSprite*>(_guiMobs.manager.get<ADrawable*>("sprite"))->setPosition(
                            sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first,
                                (*x).second->manager.get<std::pair<float, float> >("position").second));
                    _guiMobs.manager.get<ADrawable*>("sprite")->update(duration);
                    _win.draw(_guiMobs);
                }
                else if ((*x).second->manager.get<std::string>("type") == "mob")
                {
                    if ((*x).second->manager.get<std::string>("name") == "mob1")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[4]));
                    else if ((*x).second->manager.get<std::string>("name") == "mob2")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[5]));
                    else if ((*x).second->manager.get<std::string>("name") == "mob3")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[6]));
                    static_cast<AnimatedSprite*>(_guiMobs.manager.get<ADrawable*>("sprite"))->setPosition(
                            sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first,
                                (*x).second->manager.get<std::pair<float, float> >("position").second));
                    _guiMobs.manager.get<ADrawable*>("sprite")->update(duration);
                    _win.draw(_guiMobs);
                }
                else if ((*x).second->manager.get<std::string>("type") == "bonus")
                {
                    if ((*x).second->manager.get<std::string>("name") == "force")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[7]));
                    else if ((*x).second->manager.get<std::string>("name") == "shield")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[8]));
                    else if ((*x).second->manager.get<std::string>("name") == "perfect_shield")
                        _guiMobs.manager.set<ADrawable*>("sprite", &(_pSprites[9]));
                    static_cast<AnimatedSprite*>(_guiMobs.manager.get<ADrawable*>("sprite"))->setPosition(
                            sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first + 115,
                                (*x).second->manager.get<std::pair<float, float> >("position").second));
                    _guiMobs.manager.get<ADrawable*>("sprite")->update(duration);
                    _win.draw(_guiMobs);
                }
                else if ((*x).second->manager.get<std::string>("type") == "explosion")
                {
                    AnimatedSprite *ex = new AnimatedSprite;

                    if (ex->load("client/res/explosion_128.png", false, 24))
                    {
                        _explosions.push_back(ex);

                        ex->setPosition(sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first,
                                    (*x).second->manager.get<std::pair<float, float> >("position").second));
                    }
                    else
                        delete ex;
                    x = _entities->erase(x);
                    has_been_del = true;
                }
                else if ((*x).second->manager.get<std::string>("type") == "player" && (*x).second->manager.get<int>("lifes") > 0)
                {
                    _players[(*x).second->manager.get<std::string>("pseudo")]->setPosition(
                            sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first,
                                (*x).second->manager.get<std::pair<float, float> >("position").second));
                    _players[(*x).second->manager.get<std::string>("pseudo")]->update(duration);
                    _guiPlayers.manager.set<ADrawable*>("player", _players[(*x).second->manager.get<std::string>("pseudo")]);
                    if ((*x).second->manager.get<int>("respawn") > 0)
                    {
                        static bool print = false;
                        print = !print;
                        if (print)
                            _win.draw(_guiPlayers);
                    }
                    else
                        _win.draw(_guiPlayers);
                    if ((*x).second->manager.get<bool>("force"))
                    {
                        _pSprites[7].setPosition(
                                sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first + 111,
                                    (*x).second->manager.get<std::pair<float, float> >("position").second - 5));
                        _pSprites[7].update(duration);
                        _guiPlayers.manager.set<ADrawable*>("player", &(_pSprites[7]));
                        _win.draw(_guiPlayers);
                    }
                    if ((*x).second->manager.get<int>("shield") > 0)
                    {
                        _pSprites[8].setPosition(
                                sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first - 10,
                                    (*x).second->manager.get<std::pair<float, float> >("position").second - 35));
                        _pSprites[8].update(duration);
                        _guiPlayers.manager.set<ADrawable*>("player", &(_pSprites[8]));
                        _win.draw(_guiPlayers);
                    }
                    if ((*x).second->manager.get<int>("perfect_shield") > 0)
                    {
                        _pSprites[9].setPosition(
                                sf::Vector2f((*x).second->manager.get<std::pair<float, float> >("position").first + 22,
                                    (*x).second->manager.get<std::pair<float, float> >("position").second));
                        _pSprites[9].update(duration);
                        _guiPlayers.manager.set<ADrawable*>("player", &(_pSprites[9]));
                        _win.draw(_guiPlayers);
                    }
                }
                if (!has_been_del)
                    ++x;
            }
            for (uint64_t i = 0; i < _explosions.size(); ++i)
            {
                _explosions[i]->update(duration);
                if (static_cast<AnimatedSprite*>(_explosions[i])->getNbPlayed())
                {
                    delete _explosions[i];
                    _explosions[i] = 0;
                    _explosions.erase(_explosions.begin() + i);
                    continue ;
                }
                _guiExplosion.manager.set<ADrawable*>("explosion", _explosions[i]);
                _win.draw(_guiExplosion);
            }
            std::string tmp;
            for (auto x : *_entities)
                if (x.first < 1000000000)
                {
                    if (tmp.empty() == false)
                        tmp += "    ";
                    tmp += x.second->manager.get<std::string>("pseudo") + " " + std::to_string(x.second->manager.get<int>("lifes"));
                }
            _hudText.setText(tmp);
            _win.draw(_b5);
            _win.draw(_b6);
            _win.draw(_b7);
        }

        virtual void        in(IPacket *p, std::string&)
        {
            UdpPacket   *packet;

            if ((packet = dynamic_cast<UdpPacket*>(p))
                    && packet->getQuery() == static_cast<uint16_t>(UdpCodes::NewPos))
            {
                std::string tmp = std::string(
                        static_cast<const char *>(packet->getData()), packet->getSize());
                float px, py;
                std::string name = tmp.substr(0, tmp.find(":"));
                tmp = tmp.substr(tmp.find(":") + 1);
                px = static_cast<float>(std::atof(tmp.substr(0, tmp.find(":")).c_str()));
                py = static_cast<float>(std::atof(tmp.substr(tmp.find(":") + 1).c_str()));
                if (_lastId < packet->getID())
                {
                    _lastId = packet->getID();
                    if (_players[name] && _players[name] != &(_pSprites[0]))
                        _players[name]->setPosition(sf::Vector2f(px, py));
                }
            }
        }

    private:
        Entity                                              _b1;
        Entity                                              _b2;
        Entity                                              _b3;
        Entity                                              _b4;
        Entity                                              _b5;
        Entity                                              _b6;
        Entity                                              _b7;
        Entity                                              _guiPlayers;
        Entity                                              _guiShoots;
        Entity                                              _guiShootsEnnemy;
        Entity                                              _guiMobs;
        Entity                                              _guiExplosion;
        Entity                                              _guiBonuses;
        Entity                                              _changeScene;

        View                                                _view;
        std::vector<ScrollingSprite*>                       _s1;
        std::vector<ScrollingSprite*>                       _s2;
        std::vector<ScrollingSprite*>                       _s3;
        std::vector<ScrollingSprite*>                       _s4;
        std::vector<ScrollingSprite*>                       _s5;
        int                                                 _stageNb;
        std::vector<AnimatedSprite>                         _pSprites;
        AnimatedSprite                                      _shoot;
        AnimatedSprite                                      _shootEnnemy;
        AnimatedSprite                                      _transition;
        AnimatedSprite                                      _hud;
        Text                                                _hudText;
        std::vector<AnimatedSprite*>                        _explosions;

        EventSum                                            _direction;
        uint64_t                                            _lastId;
        std::unordered_map<std::string, AnimatedSprite*>    _players;

        AnimatedSprite                                      _stage;
        std::vector<AnimatedSprite>                         _numstage;
        int                                                 _durationAnimation;
        bool                                                _changing;
};


#endif /* end of include guard: STAGESCENE_HH_0PY2FK7L */
