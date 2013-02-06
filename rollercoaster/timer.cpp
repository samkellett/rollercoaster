#include "timer.h"

Timer::Timer() :
  started_(false)
{
}

Timer::~Timer()
{
}

void Timer::start()
{
  started_ = true;
  QueryPerformanceCounter(&t1_);
}

double Timer::elapsed()
{
  if (!started_) {
    return 0.0;
  }

  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);

  QueryPerformanceCounter(&t2_);
  return (double) (t2_.QuadPart - t1_.QuadPart) * 1000.0f / frequency.QuadPart;
}
