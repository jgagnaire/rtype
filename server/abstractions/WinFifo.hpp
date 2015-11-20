#pragma once
# include <windows.h>

template <typename PUT, typename GET = PUT, typename FILE_HANDLE = int>
class WinFifo : public IFifo<PUT, GET, FILE_HANDLE> {
private:
  HANDLE	_fdin;
  HANDLE	_fdout;
  std::string	_s1;
  std::string	_s2;

  void createWinFifo(Enum::io_flag f) {
    if (f == Enum::RD_TO_WR)
      {
	if ((_fdin = ::CreateNamedPipe(_s1.c_str(),
				       PIPE_ACCESS_INBOUND,
				       PIPE_READMODE_BYTE |
				       PIPE_TYPE_BYTE |
				       PIPE_WAIT,
				       1,
				       255, //TODO, magic number;
				       255, //TODO, magic number;
				       INIFINITE,
				       0)) == NULL)
	  throw (FifoException("open failed"));
	if ((_fdout = ::CreateNamedPipe(_s2.c_str(),
					PIPE_ACCESS_OUTBOUND,
					PIPE_READMODE_BYTE |
					PIPE_TYPE_BYTE |
					PIPE_WAIT,
					1,
					255, //TODO, magic number;
					255, //TODO, magic number;
					INIFINITE,
					0)) == NULL)
	  throw (FifoException("open failed"));
      }
    else
      {
	if ((_fdout = ::CreateNamedPipe(_s1.c_str(),
					PIPE_ACCESS_OUTBOUND,
					PIPE_READMODE_BYTE |
					PIPE_TYPE_BYTE |
					PIPE_WAIT,
					1,
					255, //TODO, magic number;
					255, //TODO, magic number;
					INIFINITE,
					0)) == NULL)
	  throw (FifoException("open failed"));
	if ((_fdin = ::CreateNamedPipe(_s2.c_str(),
				       PIPE_ACCESS_OUTBOUND,
				       PIPE_READMODE_BYTE |
				       PIPE_TYPE_BYTE |
				       PIPE_WAIT,
				       1,
				       255, //TODO, magic number;
				       255, //TODO, magic number;
				       INIFINITE,
				       0)) == NULL)
	  throw (FifoException("open failed"));
      }
  }

public:
  WinFifo() {
    CreateDirectory(".\\pipe\\", NULL);
    _s1 += ".\\pipe\\";
    _s2 += ".\\pipe\\";
    _fdin = 0;
    _fdout = 0;
  }

  virtual ~WinFifo() {
    closeUnixFifo();
  }

  virtual void	closeFds() {
    if (_fdin != 0)
      ::CloseHandle(_fdin);
    if (_fdout != 0)
      ::CloseHandle(_fdout);
    _fdin = 0;
    _fdout = 0;
  }

  virtual void	closeFifo() {
    closeFds()
    removeUnixFifo();
  }
  
  void	removeWinFifo() {
    if (_s1 != ".\\pipe\\")
      ::DeleteFile(_s1.c_str());
    if (!_s2 != ".\\pipe\\")
      ::DeleteFile(_s2.c_str());
    _s1 += ".\\pipe\\";
    _s2 += ".\\pipe\\";
  }

  virtual void	communicationBetween(const std::string &s1,
				     const std::string &s2, Enum::io_flag f) {
    _s1 = s1;
    _s2 = s2;
    if ((_s1 != ".\\pipe\\") &&
	(_s2 != ".\\pipe\\")) {
      _s1 += s1;
      _s2 += s2;
      createWinFifo(f);
    }
    else
      throw (FifoException("CreateFifo failed"));
  }

  virtual bool	operator>>(const PUT &s) {
    DWORD       ret;

    ::WriteFile(_fdout, &s, sizeof(s), &ret, 0);
    return (!(ret <= 0));
  }

  virtual bool	operator<<(GET &val) const {
    DWORD       ret;

    ::ReadFile(_fdin, &val, sizeof(val), &ret, 0);
    return (!(ret <= 0));
  }

  virtual const std::string &getFirstName() const { return (_s1); }

  virtual const std::string &getSecondName() const { return (_s2); }

  virtual HANDLE	getFdIn() const { return (_fdin); }

  virtual HANDLE	getFdOut() const { return (_fdout); }
};


template <>
class WinFifo :
  public IFifo<std::string, std::string, HANDLE> {
private:
  HANDLE	_fdin;
  HANDLE	_fdout;
  std::string	_s1;
  std::string	_s2;

  void createWinFifo(Enum::io_flag f) {
    if (f == Enum::RD_TO_WR)
      {
	if ((_fdin = ::CreateNamedPipe(_s1.c_str(),
				       PIPE_ACCESS_INBOUND,
				       PIPE_READMODE_BYTE |
				       PIPE_TYPE_BYTE |
				       PIPE_WAIT,
				       1,
				       255, //TODO, magic number;
				       255, //TODO, magic number;
				       INIFINITE,
				       0)) == NULL)
	  throw (FifoException("open failed"));
	if ((_fdout = ::CreateNamedPipe(_s2.c_str(),
					PIPE_ACCESS_OUTBOUND,
					PIPE_READMODE_BYTE |
					PIPE_TYPE_BYTE |
					PIPE_WAIT,
					1,
					255, //TODO, magic number;
					255, //TODO, magic number;
					INIFINITE,
					0)) == NULL)
	  throw (FifoException("open failed"));
      }
    else
      {
	if ((_fdout = ::CreateNamedPipe(_s1.c_str(),
					PIPE_ACCESS_OUTBOUND,
					PIPE_READMODE_BYTE |
					PIPE_TYPE_BYTE |
					PIPE_WAIT,
					1,
					255, //TODO, magic number;
					255, //TODO, magic number;
					INIFINITE,
					0)) == NULL)
	  throw (FifoException("open failed"));
	if ((_fdin = ::CreateNamedPipe(_s2.c_str(),
				       PIPE_ACCESS_OUTBOUND,
				       PIPE_READMODE_BYTE |
				       PIPE_TYPE_BYTE |
				       PIPE_WAIT,
				       1,
				       255, //TODO, magic number;
				       255, //TODO, magic number;
				       INIFINITE,
				       0)) == NULL)
	  throw (FifoException("open failed"));
      }
  }

public:
  WinFifo() {
    CreateDirectory(".\\pipe\\", NULL);
    _s1 += ".\\pipe\\";
    _s2 += ".\\pipe\\";
    _fdin = 0;
    _fdout = 0;
  }

  virtual ~WinFifo() {
    closeUnixFifo();
  }

  virtual void	closeFds() {
    if (_fdin != 0)
      ::CloseHandle(_fdin);
    if (_fdout != 0)
      ::CloseHandle(_fdout);
    _fdin = 0;
    _fdout = 0;
  }

  virtual void	closeFifo() {
    closeFds()
    removeUnixFifo();
  }
  
  void	removeWinFifo() {
    if (_s1 != ".\\pipe\\")
      ::DeleteFile(_s1.c_str());
    if (!_s2 != ".\\pipe\\")
      ::DeleteFile(_s2.c_str());
    _s1 += ".\\pipe\\";
    _s2 += ".\\pipe\\";
  }

  virtual void	communicationBetween(const std::string &s1,
				     const std::string &s2, Enum::io_flag f) {
    _s1 = s1;
    _s2 = s2;
    if ((_s1 != ".\\pipe\\") &&
	(_s2 != ".\\pipe\\")) {
      _s1 += s1;
      _s2 += s2;
      createWinFifo(f);
    }
    else
      throw (FifoException("CreateFifo failed"));
  }

virtual bool	operator>>(const std::string &s) {
    DWORD       ret;

    ::WriteFile(_fdout, &s, sizeof(s), &ret, 0);
    return (!(ret <= 0));
  }

virtual bool	operator<<(std::string &val) const {
    DWORD       ret;

    ::ReadFile(_fdin, &val, sizeof(val), &ret, 0);
    return (!(ret <= 0));
  }

  virtual const std::string &getFirstName() const { return (_s1); }

  virtual const std::string &getSecondName() const { return (_s2); }

  virtual HANDLE	getFdIn() const { return (_fdin); }

  virtual HANDLE	getFdOut() const { return (_fdout); }
};
