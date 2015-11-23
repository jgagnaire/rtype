#ifndef IDRAWABLE_HH_
# define IDRAWABLE_HH_

# include <vector>

class ADrawable
{
    public:
        ADrawable() {}
        virtual ~ADrawable() {}

        virtual void    update() = 0;

        virtual const void  *getBuffer() const
        {
             return _buffer;
        }

    protected:
        const void  *_buffer;
};

#endif /* ifndef IDRAWABLE_HH_ */
