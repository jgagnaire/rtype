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
                    ea->update();
                    x.second->update(*this->clk);
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
