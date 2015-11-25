#include <iostream>
#include <cstdint>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"
#include "System/Render/Window.hh"
#include "System/Render/Event.hh"
#include "Entity/Entity.hh"
#include "Component/Component.hh"
#include "System/Render/AnimatedSprite.hh"
#include "System/Render/View.hh"
#include "System/Render/Text.hh"

int main(int ac, char **av)
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    if (ac == 1)
    {
        std::cout << "Please put a argument : " << std::endl
            << "1 : Network" << std::endl
            << "2 : Graphics" << std::endl;
        return 0;
    }
    Entity e;

    AnimatedSprite  arrow;
    arrow.load("client/res/menu/arrow_334.png");
    arrow.setPosition(sf::Vector2f(1500, 900));
    e.manager.add<ADrawable*>("tmp2", &arrow);

    AnimatedSprite  spaceShip;
    spaceShip.load("client/res/ship/player-ship_128.png");
    e.manager.add<ADrawable*>("space", &spaceShip);

    std::vector<Text*>       vec;

    vec.push_back(new Text("Play Online"));
    vec.push_back(new Text("Play Offline"));
    vec.push_back(new Text("Settings"));
    vec.push_back(new Text("Quit"));
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        vec[i]->setCenter();
        vec[i]->setY(400 + i * 100);
        e.manager.add<ADrawable*>("text" + std::to_string(i), vec[i]);
    }

    AnimatedSprite  selector;
    selector.load("client/res/menu/rect-select_450.png", true);
    selector.setPosition(sf::Vector2f(735, 375));
    e.manager.add<ADrawable*>("selector", &selector);


    if (ac > 5 && av[1][0] == '1')
    {
        NetworkManager nm(av[2], std::atoi(av[3]),
                av[4], std::atoi(av[5]));
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
        int     current = 0;
        while (win->isOpen())
        {
            IEvent *event = new Event();
            while (win->getEvent(*event))
            {
                if (event->isCloseWindow())
                    win->close();
                if (event->isAccepted())
                    std::cout << "I Accept" << std::endl;
                if (event->isUp() && current > 0)
                {
                     --current;
                     selector.setPosition(sf::Vector2f(selector.getPosition().x,
                             selector.getPosition().y - 100));
                }
                if (event->isDown() && current  < vec.size() - 1)
                {
                     ++current;
                     selector.setPosition(sf::Vector2f(selector.getPosition().x,
                             selector.getPosition().y + 100));
                }
            }
            delete event;
            win->clear();
            for (auto x : e.manager.getAll<ADrawable*>())
            {
                 x->update();
            }
            win->draw(e);
            win->display();
        }
        delete win;
    }
    return 0;
}
