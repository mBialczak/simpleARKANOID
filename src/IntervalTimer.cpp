#include "IntervalTimer.hpp"

// constructor
IntervalTimer::IntervalTimer()
    : previous_time_(SDL_GetTicks())
    , current_time_(previous_time_)
{
}

// updates timer and returns time difference since last check
// unit of the returned interval: seconds
float IntervalTimer::UpdateAndGetInterval()
{
  // if the timer was paused - unpause
  // and reset the timer
  if (paused_) {
    paused_ = false;
    current_time_ = previous_time_ = SDL_GetTicks();
  }

  // get current clock ticks
  current_time_ = SDL_GetTicks();
  // count the ticks difference from previous measurement
  Uint32 interval = current_time_ - previous_time_;
  // start new "lap"
  previous_time_ = current_time_;
  // convert the clock ticks and return seconds
  return static_cast<float>(interval) / ticks_per_second;
}