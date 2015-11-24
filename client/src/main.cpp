#include <iostream>
#include <cstdint>
#include "NetworkManager.hh"
#include "UdpSocket.hh"
#include "TcpSocket.hh"
#include "Graphics/Window.hh"
#include "Graphics/Event.hh"
#include "Entity/Entity.hh"
#include "Component/Component.hh"
#include "AnimatedSprite.hh"
#include "Graphics/View.hh"
#include "Graphics/Text.hh"

int main(int ac, char **av)
{
    std::srand(std::time(0));
    if (ac == 1)
    {
        std::cout << "Please put a argument : " << std::endl
            << "1 : Network" << std::endl
            << "2 : Graphics" << std::endl;
        return 0;
    }
    Entity e;

    View            view;
    e.manager.add<AView*>("view", &view);

    AnimatedSprite  title;
    title.load("client/res/menu/rtype-title_835.png", true);
    title.setPosition(sf::Vector2f(542, 100));
    e.manager.add<ADrawable*>("tmp1", &title);

    AnimatedSprite  arrow;
    arrow.load("client/res/menu/arrow_334.png");
    arrow.setPosition(sf::Vector2f(1500, 900));
    e.manager.add<ADrawable*>("tmp2", &arrow);

    std::vector<Text*>       vec;

    vec.push_back(new Text("Play Online"));
    vec.push_back(new Text("Play Offline"));
    vec.push_back(new Text("Settings"));
    vec.push_back(new Text("Quit"));
    vec.push_back(new Text("Quit2"));
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        vec[i]->setCenter();
        vec[i]->setY(400 + i * 100);
        e.manager.add<ADrawable*>("text" + std::to_string(i), vec[i]);
    }

    Entity back;
    AnimatedSprite  background;
    background.load("client/res/menu/background_1920.png");
    back.manager.add<ADrawable*>("tmp3", &background);

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
        while (win->isOpen())
        {
            IEvent *event = new Event();
            while (win->getEvent(*event))
            {
                if (event->isCloseWindow())
                    win->close();
                if (event->isAccepted())
                    std::cout << "I Accept" << std::endl;
            }
            delete event;
            win->clear();
            for (auto x : e.manager.getAll<ADrawable*>())
            {
                 x->update();
            }
            win->draw(back);
            win->draw(e);
            win->display();
        }
        delete win;
    }
    return 0;
}
