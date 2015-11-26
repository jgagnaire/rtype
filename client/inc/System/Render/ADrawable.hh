#ifndef ADRAWABLE_HH_
# define ADRAWABLE_HH_

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

#endif /* ifndef ADRAWABLE_HH_ */
