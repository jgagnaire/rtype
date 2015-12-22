#ifndef TEXT_HH_
# define TEXT_HH_

# include <SFML/Graphics.hpp>
# include "System/Render/ADrawable.hh"

class Text : public ADrawable
{
    public:
        Text(const std::string &text = "", uint32_t colorCode = 0xffffffff) {
            _buffer = 0;
            if (_font.loadFromFile("client/res/pixel-font.ttf"))
            {
                _text.setFont(_font);
                _text.setString(text);
                _text.setColor(sf::Color(colorCode));
                _text.setStyle(sf::Text::Regular);
                _text.setCharacterSize(50);
                _buffer = &_text;
            }
        }
        virtual ~Text() {
        }

        virtual void    update(uint64_t)
        {

        }

        void            setColor(uint32_t color)
        {
            _text.setColor(sf::Color(color));
        }

        void            setText(const std::string &str)
        {
            _text.setString(str);
        }

        void            setCenter()
        {
            _text.setPosition(960 - _text.getGlobalBounds().width / 2,
                    _text.getPosition().y);
        }

        void            setPosition(const uint64_t x, const uint64_t y)
        {
			_text.setPosition(static_cast<float>(x), static_cast<float>(y));
        }

        void            setY(const uint64_t y)
        {
			_text.setPosition(static_cast<float>(_text.getPosition().x), static_cast<float>(y));
        }

    private:
        sf::Font        _font;
        sf::Text        _text;
};

#endif /* ifndef TEXT_HH_ */
