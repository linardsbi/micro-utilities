#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <Arduino.h>
#include <utility>

#ifndef DEBUG
constexpr bool DEBUG = false;
#endif

template <typename T> concept BasicStreamType = requires(T m) { m.print(""); }; // First parameter must be a type that can be printed to
template <typename K> concept BasicPrintableType = requires(K m) { String(m); };

class Logger {
public:
  Logger() = delete;
  template <BasicStreamType stream, BasicPrintableType... Args>
  static constexpr inline void print(stream &str, Args &&... args) {
    if constexpr (DEBUG) {
      (str.print(std::forward<Args>(args)), ...);
    }
  }

  template <BasicPrintableType... Args>
  static inline void log(Args &&... args) {
    print(Serial, args...);
  }
};

#endif
