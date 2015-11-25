#include <iostream>
#include <cstdint>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"
#include "System/Render/Window.hh"
#include "System/Render/Event.hh"
#include "Entity/Entity.hh"
#include "Component/Component.hh"
#include "System/Render/MenuScene.hh"

int main(int ac, char **av)
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    if (ac == 1)
    {
        std::cout << "Please put a argument : " << std::endl
            << "1 : Network [sever_ip]" << std::endl
            << "2 : Graphics" << std::endl;
        return 0;
    }

    if (ac > 2 && av[1][0] == '1')
    {
        NetworkManager nm(av[2], av[2]);
        UdpPacket tosend;
        TcpPacket tosend2;

        tosend.setID(1);
        tosend.setData(const_cast<char*>("coucou"));
        tosend.setSize(6);
        tosend.setQuery(2);

        tosend2.setData(const_cast<char*>("salut"));
        tosend2.setSize(5);
        tosend2.setQuery(3);
        nm.send(tosend);
        nm.send(tosend2);
        while (1)
        {
            IPacket *tmp = nm.getPacket();
            UdpPacket *udp = 0;
            TcpPacket *tcp = 0;
            if ((udp = dynamic_cast<UdpPacket*>(tmp)))
            {
                std::cout << "Udp Packet" << std::endl
                    << "Id " << udp->getID()
                    << " query " << udp->getQuery()
                    << " size " << udp->getSize()
                    << " data [";
                std::cout.flush();
                std::cout.write(reinterpret_cast<char*>(udp->getData()), udp->getSize());
                std::cout << "]" << std::endl;
                nm.send(tosend);
            }
            else if ((tcp = dynamic_cast<TcpPacket*>(tmp)))
            {
                std::cout << "Tcp Packet" << std::endl
                    << "query " << tcp->getQuery()
                    << " size " << tcp->getSize()
                    << " data [";
                std::cout.flush();
                std::cout.write(reinterpret_cast<char*>(tcp->getData()), tcp->getSize());
                std::cout << "]" << std::endl;
                nm.send(tosend2);
            }
        }
    }
    else if (av[1][0] == '2')
    {
        Window w;
        IWindow &win = w;
        int     current = 0;
        MenuScene   ms;

        while (win.isOpen())
        {
            Event e;
            IEvent &event = e;
            while (win.getEvent(event))
            {
                if (event.isCloseWindow())
                    win.close();
                if (event.isAccepted())
                    std::cout << "I Accept" << std::endl;
            }
            win.clear();
            ms.update();
            for (auto x : ms.getEntities())
                win.draw(*x);
            win.display();
        }
    }
    return 0;
}
