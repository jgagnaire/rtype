#ifndef ASYSTEM_HH_
# define ASYSTEM_HH_

#include <vector>
#include "Event/EventAggregator.hh"
#include "Network/IPacket.hh"
#include "Utility/IClock.hh"

class ASystem
{
    public:
        ASystem() {}
        virtual ~ASystem() {}

        virtual void                    update(IClock &clock) = 0;
        virtual IPacket                 *out() = 0;
        virtual void                    in(IPacket*) = 0;
        virtual bool                    handle(REvents e) = 0;
        virtual std::vector<REvent>     broadcast(void) = 0;
        virtual REvents                 getEvent() = 0;
    protected:
        std::vector<REvent> _eventList;
};

#endif //ASYSTEM_HH_
