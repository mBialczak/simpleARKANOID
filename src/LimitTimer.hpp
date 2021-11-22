#ifndef LimitTimer_HPP
#define LimitTimer_HPP
#include "SDL.h"

class LimitTimer {
  public:
  // constructor starting timer
  // takes time to measure in milliseconds
  explicit LimitTimer(const Uint32 timeToMeasure);

  // long TimeSinceStart() const;//REVIEW:

  void waitTillExpire() const;

  bool hasExpired() const;

  // restarts the timer
  void restart();

  private:
  Uint32 start_time_point;
  Uint32 time_to_measure;
  Uint32 expiry_time_point;
};

#endif // !LimitTimer_HPP