#ifndef TIMER_HPP
#define TIMEER_HPP
#include <chrono>

class Timer {
  public:
  // constructor
  explicit Timer(const std::chrono::microseconds timeInterval);

  long TimeSinceStart() const;

  void waitTillExpire() const;

  bool hasExpired() const;

  // restarts the timer
  void restart();

  private:
  std::chrono::time_point<std::chrono::steady_clock> start_time;
  std::chrono::microseconds time_interval;
  std::chrono::time_point<std::chrono::steady_clock> expiry_time;
};

#endif // !TIMER_HPP