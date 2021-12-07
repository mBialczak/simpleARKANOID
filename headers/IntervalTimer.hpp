#ifndef IntervalTimer_HPP
#define IntervalTimer_HPP

#include "SDL.h"
class IntervalTimer
{
  public:
  // constructor
  IntervalTimer();

  // updates timer and returns time difference since last check
  // unit of the returned interval: seconds
  float UpdateAndGetInterval();

  // pauses the timer
  void Pause() { _paused = true; };

  // copying of class object makes no sense,
  // so copy operations are disabled
  IntervalTimer(const IntervalTimer&) = delete;
  IntervalTimer& operator=(const IntervalTimer&) = delete;

  private:
  static constexpr float _ticks_per_second = 1000.0f;
  Uint32 _previous_time;
  Uint32 _current_time;
  // indicates if the timer is paused
  bool _paused;
};
#endif // !IntervalTimer_HPP