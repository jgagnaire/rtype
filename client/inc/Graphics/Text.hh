#ifndef TEXT_HH_
# define TEXT_HH_

# include <SFML/Graphics.hpp>
# include "Graphics/ADrawable.hh"

class Text : public ADrawable
{
    public:
        Text(const std::string &text) {
            _buffer = 0;
            if (_font.loadFromFile("client/res/pixel-font.ttf"))
            {
                _text.setFont(_font);
                _text.setString(text);
                _text.setColor(sf::Color::Red);
                _text.setStyle(sf::Text::Regular);
                _text.setCharacterSize(50);
                _buffer = &_text;
            }
        }
        virtual ~Text() {
        }

        virtual void    update()
        {

        }

        void            setCenter()
        {
            _text.setPosition(960 - _text.getGlobalBounds().width / 2,
                    _text.getPosition().y);
        }

        void            setPosition(const std::size_t x, const std::size_t y)
        {
             _text.setPosition(x, y);
        }

        void            setY(const std::size_t y)
        {
             _text.setPosition(_text.getPosition().x, y);
        }

    private:
        sf::Font        _font;
        sf::Text        _text;
};

#endif /* ifndef TEXT_HH_ */
