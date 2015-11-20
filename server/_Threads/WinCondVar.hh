#pragma once

#include <windows.h>
#include "Enum.hh"
#include "ICondVar.hh"

class WinCondVar : public ICondVar {
public:
  WinCondVar();
  virtual ~WinCondVar() {
  }
  virtual void wait(void);
  virtual void signal(void);
  virtual void broadcast(void);
private:
  CRITICAL_SECTION	cs;
  CONDITION_VARIABLE	cv;
};
