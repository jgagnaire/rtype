#include "WinCondVar.hh"

WinCondVar::WinCondVar() {
  ::InitializeCriticalSection(&cs);
  ::InitializeConditionVariable(&cv);
}

WinCondVar::~WinCondVar() { ::WakeAllConditionVariable(&cv); }

void WinCondVar::wait(void) { ::SleepConditionVariableCS(&cv, &cs, INFINITE); }

void WinCondVar::signal(void) { ::WakeConditionVariable(&cv); }

void WinCondVar::broadcast(void) { ::WakeAllConditionVariable(&cv); }
