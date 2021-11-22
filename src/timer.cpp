#include "timer.hpp"
#include <thread>
using namespace std::chrono_literals;

// constructor ; starts the timer at construction
Timer::Timer(const std::chrono::microseconds timeInterval)
    : start_time(std::chrono::steady_clock::now())
    , time_interval(timeInterval)
    , expiry_time(start_time + time_interval)
{
}

// returns time which passed since timer start
long Timer::TimeSinceStart() const
{
  auto now = std::chrono::steady_clock::now();
  auto diff = now - start_time;
  return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
}
// checks if desired time interval sinsce start passed
bool Timer::hasExpired() const
{
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::steady_clock::now() - start_time)
      <= time_interval;
  // return std::chrono::milliseconds(std::chrono::steady_clock::now())
  //     <= expiry_time;
}

// stops execution until desired time interval since start will pass
void Timer::waitTillExpire() const
{
  if (!hasExpired()) {
    std::this_thread::sleep_for(expiry_time - std::chrono::steady_clock::now());
  }
}

// restarts the timer
void Timer::restart()
{
  start_time = std::chrono::steady_clock::now();
  expiry_time = start_time + time_interval;
}