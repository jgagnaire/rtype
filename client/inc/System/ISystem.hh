#ifndef ISYSTEM_HH_
# define ISYSTEM_HH_

#include <vector>
#include "Event/EventAggregator.hh"
#include "Network/IPacket.hh"
#include "Utility/IClock.hh"

class ISystem
{
    public:
        ISystem() {}
        virtual ~ISystem() {}

        virtual void                    update(IClock &clock) = 0;
        virtual IPacket                 *out() = 0;
        virtual void                    in(IPacket*) = 0;
        virtual bool                    handle(REvents e) = 0;
        virtual std::vector<REvent>     broadcast(void) = 0;
        virtual REvents                 getEvent() = 0;
    protected:
        std::vector<REvent> _eventList;
};

#endif //ISYSTEM_HH_
