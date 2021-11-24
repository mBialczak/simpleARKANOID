#ifndef LimitTimer_HPP
#define LimitTimer_HPP
#include "SDL.h"

class LimitTimer {
  public:
  // constructor starting timer
  // takes time to measure in milliseconds
  explicit LimitTimer(const Uint32 timeToMeasure);

  // copying of class object should be unnecassary, so copy operations are
  // disabled
  LimitTimer(const LimitTimer&) = delete;
  LimitTimer& operator=(const LimitTimer&) = delete;

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