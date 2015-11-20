#pragma once

#include "Enum.hh"
#include "ICondVar.hh"

class WinCondVar {
public:
WinCondVar();
  virtual ~WinCondVar() {}
  virtual void wait(void);
  virtual void signal(void);
  virtual void broadcast(void);
};
