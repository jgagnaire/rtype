#ifndef IPACKET_HH_
#define IPACKET_HH_

# include <cstdint>

typedef struct s_header
{
} Header;

class IPacket
{
    public:
        IPacket() {}
        virtual ~IPacket() {}

        virtual uint16_t    getSize() const = 0;
        virtual uint16_t    getQuery() const = 0;
        virtual const void  *getData() const = 0;

        virtual void        setSize(uint16_t size) = 0;
        virtual void        setQuery(uint16_t query) = 0;
        virtual void        setData(const void *data) = 0;

        virtual Header           &getHeader() = 0;
        virtual const Header     &getHeader() const = 0;

    protected:
        const void                *_data;

};

#endif /* ifndef IPACKET_HH_ */
