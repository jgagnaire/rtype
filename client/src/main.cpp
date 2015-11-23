#include <iostream>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"
#include "Graphics/Window.hh"
#include "Graphics/Event.hh"
#include "Scenes/IntroScene.hh"

int main(int ac, char **av)
{
    if (ac == 1)
    {
        std::cout << "Please put a argument : " << std::endl
            << "1 : Network" << std::endl
            << "2 : Graphics" << std::endl;
        return 0;
    }
    if (av[1][0] == '1')
    {
        NetworkManager nm;
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
        IWindow *win = new Window();
        IntroScene  scene;
        while (win->isOpen())
        {
            IEvent *e = new Event();
            while (win->getEvent(*e))
            {
                if (e->isCloseWindow())
                    win->close();
                if (e->isAccepted())
                    std::cout << "I Accept" << std::endl;
            }
            delete e;
            win->clear();
            scene.update();
            win->draw(scene);
            win->display();
        }
        delete win;
    }
    return 0;
}
