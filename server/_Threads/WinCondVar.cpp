#include "WinCondVar.hh"

WinCondVar::WinCondVar() {
  InitializeCriticalSection(&cs);
  InitializeConditionalVariable(&cv);
}

WinCondVar::~WinCondVar() { ::WakeAllConditionVariable(&cv); }

void WinCondVar::wait(void) { ::SleepConditionVariableCS(&cv, &cs, INFINITE); }

void WinCondVar::signal(void) { ::WakeConditionVariable(&cv); }

void WinCondVar::broadcast(void) { ::WakeAllConditionVariable(&cv); }
