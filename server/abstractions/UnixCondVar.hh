#ifndef COND_VAR_HH_
# define COND_VAR_HH_

# include <pthread.h>
# include "ServerError.hh" 
# include "ICondVar.hh"
# include "Enum.hh"

class UnixCondVar : public ICondVar {
private:
  pthread_mutex_t	_mutex;
  pthread_cond_t	_cond;
public:
  UnixCondVar();
  virtual ~UnixCondVar();
  virtual void wait(void);
  virtual void signal(void);
  virtual void broadcast(void);
};

#endif
