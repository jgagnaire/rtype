#ifndef AVIEW_HH_
#define AVIEW_HH_

class AView
{
    public:
        AView() {}
        virtual ~AView() {}

        inline virtual void *getBuffer() const { return _buffer; }
    protected:
        void      *_buffer;
};

#endif /* ifndef AVIEW_HH_ */
