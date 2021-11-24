#include "IntervalTimer.hpp"

// constructor
IntervalTimer::IntervalTimer()
    : _previous_time(SDL_GetTicks())
    , _current_time(_previous_time)
{
}

// updates timer and returns time difference since last check
// unit of the returned interval: seconds
float IntervalTimer::UpdateAndGetInterval()
{
  _current_time = SDL_GetTicks();
  Uint32 interval = _current_time - _previous_time;
  _previous_time = _current_time;
  return static_cast<float>(interval) / 1000.0;
}