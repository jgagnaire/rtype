# include "System/Collider/FCollision.hh"

bool        Collision::explosion(Entity&, Entity &, std::pair<float, float>&)
{
    return false;
}

bool        Collision::player(Entity &me, Entity &e, std::pair<float, float> &pos)
{
    if (e.manager.get<std::string>("type") == "bonus" ||
            e.manager.get<std::string>("type") == "shoot")
        return false;
    pos = me.manager.get<std::pair<float, float> >("position");
    return true;
}

bool        Collision::shoot(Entity &, Entity &e, std::pair<float, float> &p)
{
    if (e.manager.get<std::string>("type") == "mob")
    {
        p = e.manager.get<std::pair<float, float> >("position");
        // TODO check life
        return true;
    }
    return false;
}

bool        Collision::mob(Entity&, Entity &e, std::pair<float, float>&)
{
    if (e.manager.get<std::string>("type") == "player"
            || e.manager.get<std::string>("type") == "shoot")
    {
        // TODO check shields
        return true;
    }
    return false;
}

bool        Collision::mobShoot(Entity&, Entity &e, std::pair<float, float>&)
{
    if (e.manager.get<std::string>("type") == "player")
    {
        // TODO CHECK SHIELDS
        return true;
    }
    return false;
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
