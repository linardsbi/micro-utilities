# Micro Utilities

## Requirements

A microcontroller compiler (avr-gcc, extensa) that supports C++20 features.

### Contains

* `Interrupt.hpp` - a wrapper class for adding interrupts that supports stateful lambdas;
* `stdlib_compatibility.hpp` - standard library overrides that allow [my builds of gcc for microcontrollers](https://github.com/linardsbi/compiled-toolchains) to use some stdlib features;
* `std/unique_ptr.hpp` - basic RAII owning pointer;
* `std/String.hpp` - constexpr-ified generic Arduino String class with faster number to string conversion;
* `std/array.hpp` - std::array implementation (for use when std::array is not available);
* `Logger.hpp` - wrapper for Arduino's Serial.print() to make printing more convenient.
