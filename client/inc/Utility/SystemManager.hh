#ifndef SYSTEMMANAGER_HH_
# define SYSTEMMANAGER_HH_

#include "System/ASystem.hh"
#include "System/Render/RenderSystem.hh"
#include "System/Event/EventAggregator.hh"
#include "System/Audio/AudioCallSystem.hh"
#include "Utility/Clock.hh"
#include "Network/NetworkManager.hh"

class SystemManager
{
    public:
        SystemManager(const std::string &ip):
            _networkManager(ip, ip)
        {
            ASystem *render = new RenderSystem();
            ASystem *audioCall = new AudioCallSystem();

            systemList["render"] = render;
	    systemList["audioCall"] = audioCall;
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
                    ea->update();
                    IPacket *p = _networkManager.getPacket();
                    x.second->in(p);
                    x.second->update(*this->clk);
                    p = x.second->out();
                    if (p != 0)
                        _networkManager.send(*p);
                }
            }
        }

    private:
        std::unordered_map<std::string, ASystem*> systemList;
        EventAggregator	*ea;
        IClock		*clk;
        NetworkManager          _networkManager;
};

#endif //SYSTEMMANAGER_HH_
