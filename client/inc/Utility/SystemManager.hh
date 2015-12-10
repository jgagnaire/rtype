#ifndef SYSTEMMANAGER_HH_
# define SYSTEMMANAGER_HH_

#include "System/ISystem.hh"
#include "System/Render/RenderSystem.hh"
#include "System/Event/EventAggregator.hh"
#include "Utility/Clock.hh"
#include "Network/NetworkManager.hh"

class SystemManager
{
    public:
        SystemManager(const std::string &ip):
            _networkManager(ip, ip)
        {
            ISystem *render = new RenderSystem();
            systemList["render"] = render;
            ea = new EventAggregator(static_cast<RenderSystem*>(render)->getWindow());
            clk = new Clock();
            ea->add(render);
        }

        void gameLoop()
        {
            while (ea->getWin()->isOpen())
            {
                for (auto x : systemList)
                {
                    IPacket *p = _networkManager.getPacket();
                    (void)p; //TODO
                    ea->update();
                    x.second->in(p);
                    x.second->update(*this->clk);
                    p = x.second->out();
                    if (p != 0)
                        _networkManager.send(*p);
                }
            }
        }

    private:
        std::unordered_map<std::string, ISystem*> systemList;
        EventAggregator	*ea;
        IClock		*clk;
        NetworkManager          _networkManager;
};

#endif //SYSTEMMANAGER_HH_
