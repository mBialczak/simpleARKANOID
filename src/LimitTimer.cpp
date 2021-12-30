#include "LimitTimer.hpp"

// constructor starting timer
// takes time to measure in milliseconds
LimitTimer::LimitTimer(const Uint32 timeToMeasure)
    : start_time_point_(SDL_GetTicks())
    , time_to_measure_(timeToMeasure)
    , expiry_time_point_(start_time_point_ + timeToMeasure)
{
}

// checks if desired time interval since start has passed
bool LimitTimer::hasExpired() const
{
  return expiry_time_point_ <= SDL_GetTicks();
}

// stops execution until desired time interval since start will pass
void LimitTimer::waitTillExpire() const
{
  if (!hasExpired()) {
    SDL_Delay(expiry_time_point_ - SDL_GetTicks());
  }
}

// restarts the timer
void LimitTimer::restart()
{
  start_time_point_ = SDL_GetTicks();
  expiry_time_point_ = start_time_point_ + time_to_measure_;
}