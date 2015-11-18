#ifndef COMPENENT_H_
# define COMPENENT_H_

#include "ICompenent.hh"

template<typename Type>
class Compenent : public ICompenent
{
public:
  Compenent(const std::string&, Type);
  virtual ~Compenent();
private:
};

#endif //COMPENENT_H_
