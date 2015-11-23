#ifndef IDRAWABLE_HH_
# define IDRAWABLE_HH_

# include <vector>

class IDrawable
{
    public:
        IDrawable() {}
        virtual ~IDrawable() {}

        virtual void    update() = 0;

        virtual const std::vector<const void*> &getBuffer() const
        {
             return _buffers;
        }

    protected:
        std::vector<const void*>      _buffers;
};

#endif /* ifndef IDRAWABLE_HH_ */
