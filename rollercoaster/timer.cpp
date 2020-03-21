#include "timer.h"

#include <chrono>

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
  t1_ = std::chrono::high_resolution_clock::now();
  // QueryPerformanceCounter(&t1_);
}

double Timer::elapsed()
{
  if (!started_) {
    return 0.0;
  }

  // LARGE_INTEGER frequency;
  // QueryPerformanceFrequency(&frequency);

  // QueryPerformanceCounter(&t2_);
  // return (double) (t2_.QuadPart - t1_.QuadPart) * 1000.0f / frequency.QuadPart;

  auto t2 = std::chrono::high_resolution_clock::now();
  auto diff = t2 - t1_;
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
  return ms.count();
}
