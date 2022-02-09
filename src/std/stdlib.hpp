#include <stdint.h>
#include <concepts>

template<typename T> concept UInt = std::is_unsigned_v<T> && std::is_integral_v<T>;
template<typename T> concept Int = std::is_signed_v<T> && std::is_integral_v<T>;

template <UInt U> constexpr char* utoa(const U value, char* str, const uint8_t base = 10);
template <Int I> constexpr char* itoa(const I value, char* str, const uint8_t base = 10);
