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
  // if the timer was paused - unpause
  // and reset the timer
  if (_paused) {
    _paused = false;
    _current_time = _previous_time = SDL_GetTicks();
  }

  // get current clock ticks
  _current_time = SDL_GetTicks();
  // count the ticks difference from previous measurement
  Uint32 interval = _current_time - _previous_time;
  // start new "lap"
  _previous_time = _current_time;
  // convert the clock ticks and return seconds
  return static_cast<float>(interval) / ticks_per_second;
}