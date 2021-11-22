#include "LimitTimer.hpp"

// constructor starting timer
// takes time to measure in milliseconds
LimitTimer::LimitTimer(const Uint32 timeToMeasure)
    : _start_time_point(SDL_GetTicks())
    , _time_to_measure(timeToMeasure)
    , _expiry_time_point(_start_time_point + timeToMeasure)
{
}

// checks if desired time interval since start has passed
bool LimitTimer::hasExpired() const
{
  return _expiry_time_point <= SDL_GetTicks();
}

// stops execution until desired time interval since start will pass
void LimitTimer::waitTillExpire() const
{
  if (!hasExpired()) {
    SDL_Delay(_expiry_time_point - SDL_GetTicks());
  }
}

// restarts the timer
void LimitTimer::restart()
{
  _start_time_point = SDL_GetTicks();
  _expiry_time_point = _start_time_point + _time_to_measure;
}