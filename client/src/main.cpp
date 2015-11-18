#include <iostream>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hh"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    AnimatedSprite   arbok;

    arbok.load("arbok.png", 87);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        arbok.update();
        window.draw(arbok);
        window.display();
    }

    return 0;
}
