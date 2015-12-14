#ifndef		ENUM_H_
# define	ENUM_H_

namespace Enum
{
    enum UserStatus {
        LOBBY = 0,
        GAME_ROOM,
        GAME
    };

    enum JSON {
        NO_VAL = 0,
        IS_HASH,
        IS_STRING,
        IS_TAB,
        IS_NUM,
    };

    enum ProtocolIndex {
        TCP_INDEX = 0,
        UDP_INDEX
    };

    enum	ThreadState {
        NIL = 0,
        RUNNING,
        DEAD,
    };

    enum	io_flag {
        RD_TO_WR = 0,
        WR_TO_RD,
    };

    enum Protocol {
        TCP = 1,
        UDP = 2,
        BOTH_PROTO = 3
    };

    enum  Flag {
        NONE = 0,
        READ = 1,
        WRITE = 2,
        ACCEPT = 4,
        CONNECT = 8,
        CLOSE = 16,
    };

    enum ClientQueries {
        NONE_QUERY = 0,
        ACCOUNT_LOGIN = 100,
        ACCOUNT_REGISTER = 101,
        DISCONNECTION = 102,
        GET_ROOM = 103,
        PONG = 110,
        JOIN_RANDOM_ROOM = 200,
        JOIN_GAME_ROOM = 201,
        CREATE_ROOM = 202,
        LEAVE_ROOM = 300,
        READY = 301,
        NOT_READY = 302,
        QUIT_GAME = 200,
        CURRENT_POS = 300,
        KEY_PRESS = 301,
        AUDIO_PACKET = 302,
        TAKE_FORCE = 303,
    };

    enum ServerNotification {
        PING	= 109,
        PLAYER_JOIN = 400,
        PLAYER_LEFT = 401,
        GAME_START = 402,
        GAME_NAME = 403,
        MONSTER_SPAWNED = 400,
        MONSTER_DESTROYED = 401,
        MONSTER_FIRED = 402,
        PLAYER_KILLED = 403,
        BROADCAST_AUDIO = 404,
        USER_TAKE_FORCE = 405,
        USER_QUIT_GAME = 406,
        USER_KEYPRESS = 407,
    };

    enum ServerAnswers {
        // FOR TCP
        OK = 104,
        EUSERPASS = 500,
        EALREADY_LOGGED = 501,
        EUSER_EXIST = 502,
        EREGISTER_FORMAT = 503,
        EALREADY_ON_ROOM = 504,
        ENOT_LOGGED = 505,
        ENOT_IN_ROOM = 506,
	EALREADY_IN_GAME = 507,
        ENO_AVAILABLE_ROOM = 509,
        EROOM_NO_EXIST = 510,

        // FOR UDP
	PLAYER_POS = 408,
        ENOT_IN_GAME = 504,
        EUDP_NOT_LOGGEDIN = 506
    };

    enum ServerInfo {
        PATH_LENGTH = 20,
        REGISTER_MAX = 12,
        REGISTER_MIN = 4,
        MAX_TIME = 10,
        MAX_BUFFER_LENGTH = 512,
    };

    enum Key {
        NO_KEY = 0,
        UP = 1,
        DOWN = 2,
        LEFT = 4,
        RIGHT = 8,
        FIRE = 16,
        CHARGE = 32,
        SWITCH = 64,
    };

    enum GameInfo {
        MAX_PLAYER = 4,
        MAX_ROOM_NAME_NB = 4,
        MAX_GAME = 8,
        REFRESH_TIME = 15,
        GAME_SIZE_WIDTH = 1920,
        GAME_SIZE_HEIGHT = 1080,
    };
};

#endif		// !ENUM_H_
