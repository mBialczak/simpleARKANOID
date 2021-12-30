#ifndef IntervalTimer_HPP
#define IntervalTimer_HPP

#include "SDL.h"

// class allowing measuring interval of times
class IntervalTimer
{
  public:
  // constructor
  IntervalTimer();

  // updates timer and returns time difference since last check
  // unit of the returned interval: seconds
  float UpdateAndGetInterval();

  // pauses the timer
  void Pause() { paused_ = true; };

  // constant describing how many SDL clock ticks has each second
  static constexpr float ticks_per_second = 1000.0f;

  private:
  Uint32 previous_time_;
  Uint32 current_time_;
  // indicates if the timer is paused
  bool paused_;
};
#endif // !IntervalTimer_HPP