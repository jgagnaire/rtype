#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <exception>
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
#include "System/Mob/MobSystem.hh"

#include "AudioCallSystem.hh"
#include "Utility/Clock.hh"

#include "Utility/SystemManager.hh"

int main(int ac, char **av)
{
	try {
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
	}
	catch (std::runtime_error const &e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
    return 0;
}
