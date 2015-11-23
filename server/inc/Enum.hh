#ifndef		ENUM_H_
# define	ENUM_H_

namespace Enum
{
  enum	ThreadState
    {
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
    UDP
  };

  enum  Flag {
    NONE = 0,
    READ = 1,
    WRITE = 2,
    ACCEPT = 4,
    CONNECT = 8,
    CLOSE = 16,
  };

  enum UserStatus {
    ONLINE = 0,
    OFFLINE,
    BUSY,
    AWAY,
  };

  enum ClientQueries {
    NONE_QUERY = 0,
    ACCOUNT_LOGIN = 101,
    ACCOUNT_REGISTER = 102,
    ACCOUNT_DELETION = 103,
    DISCONNECTION = 105,
    PONG = 110,
    CONTACT_ADD = 200,
    CONTACT_DELETION = 201,
    SEND_TEXT = 300,
    AUDIO_CALL_INIT = 301,
    VIDEO_CALL_INIT = 302,
    AUDIO_CALL_ACCEPT = 303,
    VIDEO_CALL_ACCEPT = 304,
    STATUS = 305,
    AUDIO_CALL_REJECT = 306,
    VIDEO_CALL_REJECT = 307,
    HANG_UP = 308,
    HANG_UP_VIDEO = 309,
  };

  enum ServerNotification {
    USER_DELETED = 401,
    USER_ADDED = 402,
    INCOMING_AUDIO = 403,
    INCOMING_VIDEO = 404,
    INCOMING_TEXT = 405,
    REJECT_AUDIO = 406,
    REJECT_VIDEO = 407,
    CHANGE_STATUS = 408,
    AUDIO_START = 409,
    VIDEO_START = 410,
    USER_HANG_UP = 411,
    USER_VIDEO_HANG_UP = 412,
  };

  enum ServerAnswers {
    OK = 104,
    CALL_ACCEPTED = 105,
    CALLING = 106,
    LOGGED_IN = 107,
    CONTACT_LIST = 108,
    PING	= 109,
    EUSERPASS = 500,
    EUSER_EXIST = 501,
    EREGISTER_FORMAT = 502,
    ENO_EXIST = 503,
    EALREADY_ADDED = 504,
    ENOT_CALLED = 505,
    ENOT_LOGGED = 506,
    EALREADY_LOGGED = 507,
  };

  enum ServerInfo {
    PATH_LENGTH = 20,
    REGISTER_MAX = 12,
    REGISTER_MIN = 4,
    MAX_TIME = 10,
    CALL_TIME = 10,
  };
};

#endif		// !ENUM_H_
