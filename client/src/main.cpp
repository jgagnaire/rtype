#include <iostream>
#include <cstdint>
#include "Network/NetworkManager.hh"
#include "Network/UdpSocket.hh"
#include "Network/TcpSocket.hh"
#include "System/Render/Window.hh"
#include "Entity/Entity.hh"
#include "Component/Component.hh"
#include "Utility/SystemManager.hh"
#include "System/Render/MenuScene.hh"
#include "System/Render/StageScene.hh"
#include "System/Event/EventAggregator.hh"
#include "System/Shoot/ShootSystem.hh"
#include "System/Movement/Movement.hh"

#include "AudioCallSystem.hh"
#include "Utility/Clock.hh"

#include "Utility/SystemManager.hh"

int main(int ac, char **av)
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    if (ac == 1)
    {
        std::cout << "Usage : ./client_rtype <serverIp>" << std::endl;
        return 0;
    }
    else
    {
        SystemManager   sm(av[1]);

        sm.gameLoop();
    }
    return 0;
}
