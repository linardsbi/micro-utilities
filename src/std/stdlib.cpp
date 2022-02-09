#include "./stdlib.hpp"
#include <string.h>
#include <concepts>
#include <assert.h>
// credit: https://assets.ctfassets.net/oxjq45e8ilak/40Ze5OoEOpGrfParOcbVXF/1b8a361bc269347795e6f068f62de2e7/Ivan_Afanasyev_stdto_string_faster_than_light_2020_06_27_17_37_45.pdf
// and libfmt

inline void copy2(char* dst, const char* src) { memcpy(dst, src, 2); }

// std::numeric_limits<unsigned>::digits10 + 2 
constexpr char* format_hex(unsigned value, char* str, const uint8_t size = 11) {
    // todo: sanity checks for 'size'
    str += size;

    constexpr const char digits[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f'
    };

    while (value >= 0x100)
	{
	  auto number = value & 0xF;
	  value >>= 4;
	  *--str = digits[number];

	  number = value & 0xF;
	  value >>= 4;
	  *--str = digits[number];
	}
    if (value >= 0x10)
    {
        const auto number = value & 0xF;
        value >>= 4;
        *--str = digits[number];
        *--str = digits[value];
    } else {
        *--str = digits[value];
    }

    return str;
}
constexpr char* format_decimal(unsigned value, char* str, const uint8_t size = 11) {
    // todo: sanity checks for 'size'
    str += size;

    constexpr const char digits[100][2] = {
      {'0', '0'}, {'0', '1'}, {'0', '2'}, {'0', '3'}, {'0', '4'}, {'0', '5'},
      {'0', '6'}, {'0', '7'}, {'0', '8'}, {'0', '9'}, {'1', '0'}, {'1', '1'},
      {'1', '2'}, {'1', '3'}, {'1', '4'}, {'1', '5'}, {'1', '6'}, {'1', '7'},
      {'1', '8'}, {'1', '9'}, {'2', '0'}, {'2', '1'}, {'2', '2'}, {'2', '3'},
      {'2', '4'}, {'2', '5'}, {'2', '6'}, {'2', '7'}, {'2', '8'}, {'2', '9'},
      {'3', '0'}, {'3', '1'}, {'3', '2'}, {'3', '3'}, {'3', '4'}, {'3', '5'},
      {'3', '6'}, {'3', '7'}, {'3', '8'}, {'3', '9'}, {'4', '0'}, {'4', '1'},
      {'4', '2'}, {'4', '3'}, {'4', '4'}, {'4', '5'}, {'4', '6'}, {'4', '7'},
      {'4', '8'}, {'4', '9'}, {'5', '0'}, {'5', '1'}, {'5', '2'}, {'5', '3'},
      {'5', '4'}, {'5', '5'}, {'5', '6'}, {'5', '7'}, {'5', '8'}, {'5', '9'},
      {'6', '0'}, {'6', '1'}, {'6', '2'}, {'6', '3'}, {'6', '4'}, {'6', '5'},
      {'6', '6'}, {'6', '7'}, {'6', '8'}, {'6', '9'}, {'7', '0'}, {'7', '1'},
      {'7', '2'}, {'7', '3'}, {'7', '4'}, {'7', '5'}, {'7', '6'}, {'7', '7'},
      {'7', '8'}, {'7', '9'}, {'8', '0'}, {'8', '1'}, {'8', '2'}, {'8', '3'},
      {'8', '4'}, {'8', '5'}, {'8', '6'}, {'8', '7'}, {'8', '8'}, {'8', '9'},
      {'9', '0'}, {'9', '1'}, {'9', '2'}, {'9', '3'}, {'9', '4'}, {'9', '5'},
      {'9', '6'}, {'9', '7'}, {'9', '8'}, {'9', '9'}};

    while (value >= 100) {
        // Integer division is slow so do it for a group of two digits instead
        // of for every digit. The idea comes from the talk by Alexandrescu
        // "Three Optimization Tips for C++". See speed-test for a comparison.
        str -= 2;
        copy2(str, digits[value % 100]);
        value /= 100;
    }
    if (value < 10) {
        *--str = static_cast<char>('0' + value);
        return str;
    }
    str -= 2;
    copy2(str, digits[value]);

    return str;
}

constexpr char* format_octal(unsigned value, char* str, const uint8_t size = 11) {
    // todo: sanity checks for 'size'
    str += size;

    while (value >= 0100)
	{
        auto number = value & 7;
        value >>= 3;
        *--str = static_cast<char>('0' + number);
        
        number = value & 7;
        value >>= 3;
        *--str = static_cast<char>('0' + number);
	}

    if (value >= 010)
	{
        const auto number = value & 7;
        value >>= 3;
        *--str = static_cast<char>('0' + number);
        *--str = static_cast<char>('0' + value);
	} else {
        *--str = static_cast<char>('0' + value);
    }

    return str;
}

constexpr char* format_binary(unsigned value, char* str, const uint8_t size = 20) {
    // todo: sanity checks for 'size'
    str += size;

    while (value > 0) {
        *--str = static_cast<char>('0' + (value & 1));
        value >>= 1;
    }

    return str;
}

constexpr char* format(unsigned value, char* str, const uint8_t base, const uint8_t size = 11) {
    str += size;

    if (base < 2 || base > 32) { [[unlikely]]
        // throw std::runtime_error("utoa: Invalid base");
        assert(base > 2 && base < 32);
    }

    constexpr const char digits[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y', 'z'
    };

    while (value >= static_cast<unsigned>(base))
	{
	  const auto quotient = value / base;
	  const auto remainder = value % base;
	  *--str = digits[remainder];
	  value = quotient;
	}

    *--str = digits[value];

    return str;
}

    template <UInt U> constexpr char* utoa(const U value, char* str, const uint8_t base) {
        switch (base) {
            case 16:
                return format_hex(value, str);
            case 10:
                return format_decimal(value, str);
            case 8:
                return format_octal(value, str);
            case 2:
                return format_binary(value, str);
            default:
                return format(value, str, base);
        }
    }

    template <Int I> constexpr char* itoa(const I value, char* str, const uint8_t base) {
        auto abs_value = static_cast<unsigned>(value);
        const bool negative = value < 0;

        if (negative) {
            abs_value = 0 - abs_value;
        }

        auto formatted = utoa(abs_value, str, base);

        if (negative) *--formatted = '-';

        return formatted;
    }
