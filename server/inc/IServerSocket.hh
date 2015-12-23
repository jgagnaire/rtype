#ifndef ISERVERSOCKET_H_
# define ISERVERSOCKET_H_

# include <string>
# include <cstddef>

template <typename T>
class IServerSocket
{
 public:
  virtual ~IServerSocket() {}
  virtual int	absReadFromClient(char *&, unsigned, std::string * const = NULL) const = 0;
  virtual bool absWriteOnClient(char *, uint64_t, const std::string & = "", const std::string & = "") const = 0;
  virtual T getSocket() const = 0;
  virtual std::string   getIP() const = 0;
protected:
  virtual void setSocket(T) = 0;
};

#endif // !ISERVERSOCKET_H_
