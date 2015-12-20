#ifndef FCOLLISION_HH_WPBKQ7AY
#define FCOLLISION_HH_WPBKQ7AY

# include "Entity/Entity.hh"

typedef std::function<bool (Entity&, Entity&, std::pair<float, float>&)>   fCollision;

namespace Collision
{
    bool        explosion(Entity&, Entity &e, std::pair<float, float>&);
    bool        player(Entity&, Entity &e, std::pair<float, float>&);
    bool        shoot(Entity&, Entity &e, std::pair<float, float>&);
    bool        mob(Entity&, Entity &e, std::pair<float, float>&);
    bool        mobShoot(Entity&, Entity &e, std::pair<float, float>&);
    bool        bonus(Entity&, Entity &e, std::pair<float, float>&);
}

#endif /* end of include guard: FCOLLISION_HH_WPBKQ7AY */
