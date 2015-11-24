#ifndef TEXT_HH_
# define TEXT_HH_

# include <SFML/Graphics.hpp>
# include "Graphics/ADrawable.hh"

class Text : public ADrawable
{
    public:
        Text(const std::string &text) {
            _buffer = 0;
            if (_font.loadFromFile("/home/tawfik/rendu/rtype/client/res/Minecraft.tff"))
            {
                _text.setFont(_font);
                _text.setString(text);
                _text.setStyle(18);
                _text.setColor(sf::Color::Red);
                _buffer = &_text;
            }
        }
        virtual ~Text() {}

        virtual void    update()
        {

        }
    private:
        sf::Font        _font;
        sf::Text        _text;
};

#endif /* ifndef TEXT_HH_ */
