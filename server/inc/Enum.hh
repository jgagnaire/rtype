#ifndef		ENUM_H_
# define	ENUM_H_

namespace Enum
{
    enum UserStatus {
        LOBBY = 0,
        GAME_ROOM,
        GAME
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
        ACCOUNT_LOGIN = 101,
        ACCOUNT_REGISTER = 102,
        UPLOAD_IMAGE = 103,
        DISCONNECTION = 105,
        SEND_IMAGE = 106,
        IMAGE_UPLOADED = 107,
        DELETE_IMAGE = 108,
        PONG = 110,
        RETRIEVE_IMAGE = 111,
        GET_ROOM = 112,
        JOIN_RANDOM_ROOM = 200,
        JOIN_GAME_ROOM = 201,
        CREATE_ROOM = 202,
        LEAVE_ROOM = 300,
        READY = 301,
        NOT_READY = 302
    };

    enum ServerNotification {
    };

    enum TCPServerAnswers {
        OK = 104,
        LOGGED_IN = 107,
        PING	= 109,
        PLAYER_JOIN = 400,
        PLAYER_LEFT = 401,
        GAME_START = 402,
        RECV_PROFILE = 403,
        END_PROFILE = 404,
        GAME_NAME = 405,
        EUSERPASS = 500,
        EALREADY_LOGGED = 501,
        EUSER_EXIST = 502,
        EREGISTER_FORMAT = 503,
        EALREADY_ON_ROOM = 504,
        ENOT_LOGGED = 505,
        ENOT_IN_ROOM = 506,
        EIMAGE_NO_EXIST = 507,
        EIMAGE_KO = 508,
        ENO_AVAILABLE_ROOM = 509,
        EROOM_NO_EXIST = 510
    };

    enum ServerInfo {
        PATH_LENGTH = 20,
        REGISTER_MAX = 12,
        REGISTER_MIN = 4,
        MAX_TIME = 10,
        MAX_PLAYER = 4,
        MAX_ROOM_NAME_NB = 16,
        MAX_BUFFER_LENGTH = 512,
    };
};

#endif		// !ENUM_H_
