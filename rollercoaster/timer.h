#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

class Timer 
{
public:
  Timer();
  ~Timer();

  void start();
  double elapsed();

private:
  LARGE_INTEGER t1_, t2_;
  bool started_;
};

#endif
