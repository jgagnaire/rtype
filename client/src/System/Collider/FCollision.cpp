# include "System/Collider/FCollision.hh"

bool        Collision::explosion(Entity &)
{
    return false;
}

bool        Collision::player(Entity &e)
{
    if (e.manager.get<std::string>("type") == "bonus" ||
            e.manager.get<std::string>("type") == "shoot")
        return false;
    //TODO check things
    return true;
}

bool        Collision::shoot(Entity &e)
{
    if (e.manager.get<std::string>("type") == "mob")
    {
        // TODO check life
        return true;
    }
    return false;
}

bool        Collision::mob(Entity &e)
{
    if (e.manager.get<std::string>("type") == "player")
    {
        // TODO check shields
        return true;
    }
    return false;
}

bool        Collision::mobShoot(Entity &e)
{
    if (e.manager.get<std::string>("type") == "player")
    {
        // TODO CHECK SHIELDS
        return true;
    }
    return false;
}

bool        Collision::bonus(Entity &e)
{
    if (e.manager.get<std::string>("type") == "player")
    {
        // TODO give bonus
        return true;
    }
    return false;
}
