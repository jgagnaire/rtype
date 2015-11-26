#ifndef ISYSTEM_HH_
# define ISYSTEM_HH_

#include <chrono>

class ISystem
{
  ISystem() {}
  virtual ~ISystem() {}

  virtual void update(std::chrono::steady_clock) = 0;
  virtual void *out(void *) = 0; //remplacer le void* par un IPacket* quand le gros lard l'aura fait
};

#endif //ISYSTEM_HH_
