#pragma once

class ICondVar {
public:
  virtual ~ICondVar() {}
  virtual void wait(void) = 0;
  virtual void signal(void) = 0;
  virtual void broadcast(void) = 0;
};
