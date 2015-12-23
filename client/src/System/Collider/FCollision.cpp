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
            shield -= 50;
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
        me.manager.set<bool>("force", false);
    }
    return lifes == 0;
}

bool        Collision::shoot(Entity &, Entity &e, std::pair<float, float> &)
{
    if (e.manager.get<std::string>("type") == "mob" ||
            e.manager.get<std::string>("type") == "boss")
        return true;
    return false;
}

bool        Collision::mob(Entity &me, Entity &e, std::pair<float, float>&)
{
    if (e.manager.get<std::string>("type") == "player"
            || e.manager.get<std::string>("type") == "shoot")
    {
        if (e.manager.get<std::string>("type") == "shoot")
        {
             int life = me.manager.get<int>("life");
             life -= e.manager.get<int>("damage");
             me.manager.set<int>("life", life);
             if (me.manager.get<std::string>("type") == "boss")
                 std::cout << "LIFE " << life << std::endl;
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
    if (e.manager.get<std::string>("type") == "player")
    {
        if (me.manager.get<std::string>("name") == "force")
        {
            e.manager.set<bool>("force", true);
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
        return true;
    }
    return false;
}
