#ifndef LimitTimer_HPP
#define LimitTimer_HPP
#include "SDL.h"

// Simple timer class which. It  measures if the given time in milliseconds has
// passed from the timer creation and allows to halt program execution until it
// will pass
class LimitTimer
{
  public:
  // Constructor starting timer
  // takes time to measure in milliseconds
  explicit LimitTimer(const Uint32 timeToMeasure);

  // stops execution until desired time interval since start will pass
  void waitTillExpire() const;

  // checks if desired time interval since start has passed
  bool hasExpired() const;

  // restarts the timer
  void restart();

  private:
  // start point time in SDL ticks
  Uint32 _start_time_point;
  // time to measure in SDL ticks
  Uint32 _time_to_measure;
  // end point time in SDL ticks
  Uint32 _expiry_time_point;
};

#endif // !LimitTimer_HPP