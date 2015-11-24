#ifndef VIEW_HH_
# define VIEW_HH_

# include <SFML/Graphics.hpp>
# include "AView.hh"

class View : public AView
{
    public:
        View() {
            _view.reset(sf::FloatRect(0, 0, 1920, 1080));
            _buffer = &_view;
        }
        virtual ~View() {}
    private:
        sf::View        _view;
};

#endif /* ifndef VIEW_HH_ */
