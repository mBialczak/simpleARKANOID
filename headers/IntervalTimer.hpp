#ifndef IntervalTimer_HPP
#define IntervalTimer_HPP

#include "SDL.h"
class IntervalTimer {
  public:
  // constructor
  IntervalTimer();

  // updates timer and returns time difference since last check
  // unit of the returned interval: seconds
  float UpdateAndGetInterval();

  // copying of class object should be unnecassary, so copy operations are
  // disabled
  IntervalTimer(const IntervalTimer&) = delete;
  IntervalTimer& operator=(const IntervalTimer&) = delete;

  private:
  Uint32 _previous_time;
  Uint32 _current_time;
};
#endif // !IntervalTimer_HPP