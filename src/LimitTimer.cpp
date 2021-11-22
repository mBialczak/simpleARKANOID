#include "LimitTimer.hpp"
// REVIEW: check all for unneded stuff

// constructor starting timer
// takes time to measure in milliseconds
LimitTimer::LimitTimer(const Uint32 timeToMeasure)
    : start_time_point(SDL_GetTicks())
    , time_to_measure(timeToMeasure)
    , expiry_time_point(start_time_point + timeToMeasure)
{
}

// // returns time which passed since timer start
// long Timer::TimeSinceStart() const
// {
//   auto now = std::chrono::steady_clock::now();
//   auto diff = now - start_time;
//   return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
// }

// checks if desired time interval sinsce start passed
bool LimitTimer::hasExpired() const
{
  return expiry_time_point <= SDL_GetTicks();
}

// stops execution until desired time interval since start will pass
void LimitTimer::waitTillExpire() const
{
  if (!hasExpired()) {
    SDL_Delay(expiry_time_point - SDL_GetTicks());
  }
}

// restarts the timer
void LimitTimer::restart()
{
  start_time_point = SDL_GetTicks();
  expiry_time_point = start_time_point + time_to_measure;
}