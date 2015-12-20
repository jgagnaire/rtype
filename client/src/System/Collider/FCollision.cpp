# include "System/Collider/FCollision.hh"

bool        Collision::explosion(Entity&, Entity &)
{
    return false;
}

bool        Collision::player(Entity&, Entity &e)
{
    if (e.manager.get<std::string>("type") == "bonus" ||
            e.manager.get<std::string>("type") == "shoot")
        return false;
    //TODO check things
    return true;
}

bool        Collision::shoot(Entity&, Entity &e)
{
    if (e.manager.get<std::string>("type") == "mob")
    {
        // TODO check life
        return true;
    }
    return false;
}

bool        Collision::mob(Entity&, Entity &e)
{
    if (e.manager.get<std::string>("type") == "player"
            || e.manager.get<std::string>("type") == "shoot")
    {
        // TODO check shields
        return true;
    }
    return false;
}

bool        Collision::mobShoot(Entity&, Entity &e)
{
    if (e.manager.get<std::string>("type") == "player")
    {
        // TODO CHECK SHIELDS
        return true;
    }
    return false;
}

bool        Collision::bonus(Entity &me, Entity &e)
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
        else if (me.manager.get<std::string>("name") == "perfect-shield")
        {
            e.manager.set<int>("perfect-shield",
                    me.manager.get<int>("duration") * 1000);
        }
        return true;
    }
    return false;
}
