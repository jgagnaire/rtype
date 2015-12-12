#ifndef ADRAWABLE_HH_
# define ADRAWABLE_HH_

# include <cstddef>

class ADrawable
{
    public:
        ADrawable() {}
        virtual ~ADrawable() {}

        virtual void    update(std::size_t) = 0;

        inline virtual const void *getBuffer() const { return _buffer; }

    protected:
        const void  *_buffer;
};

#endif /* ifndef ADRAWABLE_HH_ */
