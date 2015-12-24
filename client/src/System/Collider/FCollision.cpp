# include "System/Collider/FCollision.hh"

bool        Collision::explosion(Entity&, Entity &, std::pair<float, float>&)
{
    return false;
}

bool        Collision::player(Entity &me, Entity &e, std::pair<float, float> &pos)
{
    int     shield;

    if (e.manager.get<std::string>("type") == "bonus" ||
            e.manager.get<std::string>("type") == "shoot")
        return false;
    if (me.manager.get<int>("perfect_shield"))
        return false;
    if ((shield = me.manager.get<int>("shield")) > 0)
    {
        if (e.manager.get<std::string>("type") == "mobshoot")
            shield -= e.manager.get<int>("damage");
        else
            shield = 0;
        if (shield < 0)
            shield = 0;
        me.manager.set<int>("shield", shield);
        return false;
    }
    int lifes = me.manager.get<int>("lifes");
    if (me.manager.get<int>("respawn") <= 0)
    {
        pos = me.manager.get<std::pair<float, float> >("position");
        me.manager.set<std::pair<float, float> >("position",
                std::pair<float, float>(0, 1080 / 2));
        --lifes;
        me.manager.set<int>("lifes", lifes);
        me.manager.set<int>("respawn", 3000);
        me.manager.set<int>("force", 1);
    }
    return false;
}

bool        Collision::shoot(Entity &, Entity &e, std::pair<float, float> &)
{
    if (e.manager.get<std::string>("type") == "mob" ||
            e.manager.get<std::string>("type") == "boss")
        return true;
    return false;
}

bool        Collision::mob(Entity &me, Entity &e, std::pair<float, float> &pos)
{
    int     score;

    if (e.manager.get<std::string>("type") == "player"
            || e.manager.get<std::string>("type") == "shoot")
    {
        if (e.manager.get<std::string>("type") == "shoot")
        {
             int life = me.manager.get<int>("life");
             life -= e.manager.get<int>("damage") * e.manager.get<Entity*>("Shooter")->manager.get<int>("force");
             me.manager.set<int>("life", life);
             pos = me.manager.get<std::pair<float, float> >("position");
             if (me.manager.get<std::string>("type") == "boss")
             {
                std::cout << "life " << life << std::endl;
             }
             if (life <= 0)
             {
                 score = e.manager.get<Entity*>("Shooter")->manager.get<uint64_t>("score");
                 score += 5 * e.manager.get<Entity*>("Shooter")->manager.get<int>("force");
                 e.manager.get<Entity*>("Shooter")->manager.set<uint64_t>("score", score);
             }
             if (me.manager.get<std::string>("type") == "boss" && life > 0)
                 pos = std::pair<float, float>(
                         me.manager.get<std::pair<float, float> >("position").first,
                         e.manager.get<std::pair<float, float> >("position").second);
             if (me.manager.get<std::string>("type") == "boss")
                 return false;
             return life <= 0;
        }
        if (me.manager.get<std::string>("type") == "boss")
            return false;
        return true;
    }
    return false;
}

bool        Collision::mobShoot(Entity&, Entity &e, std::pair<float, float>&)
{
    return (e.manager.get<std::string>("type") == "player");
}

bool        Collision::bonus(Entity &me, Entity &e, std::pair<float, float>&)
{
    uint64_t score = 0;
    if (e.manager.get<std::string>("type") == "player")
    {
        if (me.manager.get<std::string>("name") == "force")
        {
            e.manager.set<int>("force", me.manager.get<int>("power"));
        }
        else if (me.manager.get<std::string>("name") == "shield")
        {
            e.manager.set<int>("shield", me.manager.get<int>("protection"));
        }
        else if (me.manager.get<std::string>("name") == "perfect_shield")
        {
            e.manager.set<int>("perfect_shield",
                    me.manager.get<int>("duration") * 1000);
        }
        score = e.manager.get<uint64_t>("score");
        score += 12;
        e.manager.set<uint64_t>("score", score);
        return true;
    }
    return false;
}
