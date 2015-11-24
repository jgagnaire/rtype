#include "ThreadPool.hh"

void	lol(int a) {
  printf("penis %d\n", a);
}

void	lola(int a) {
  printf("vit %d\n", a);
  sleep(10);
}

int main() {
  IThreadPool<void, int > *t = new ThreadPool<void, int >();

  t->add(&lola, 5);
  t->add(&lol, 2);
  t->startAll();
  sleep(1);
  std::cout << "il y a " << t->getIdleThreadNumber() << " thread en cours" << std::endl;
  sleep(2);
  return (0);
}
