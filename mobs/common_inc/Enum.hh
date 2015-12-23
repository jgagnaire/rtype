#ifndef		ENUM_H_
# define	ENUM_H_

# if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#  define init_memory(x, y) ZeroMemory((x), (y));
# else
#  define init_memory(x, y) bzero((x), (y));
# endif

namespace Enum
{
    enum JSON {
        NO_VAL = 0,
        IS_HASH,
        IS_STRING,
        IS_TAB,
        IS_NUM,
        IS_FLOAT,
    };
};

#endif		// !ENUM_H_
