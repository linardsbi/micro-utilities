#pragma once
#include "Arduino.h"
#include "wiring_private.h"
#include <array>
#include <functional>

class Interrupt {
public:
    
    static auto &ISRS() {
        // todo: is there a way that does not use type reflection that would
        //       let the code decide whether to use std::function or just a 
        //       void function ptr?
        using void_fun_ptr = std::function<void()>;
        static std::array<void_fun_ptr, EXTERNAL_NUM_INTERRUPTS> isrs;
        return isrs;
    }

    template <size_t InterruptNum>
    static void add(const uint8_t mode, auto function) {
        static_assert(0 <= InterruptNum && InterruptNum < EXTERNAL_NUM_INTERRUPTS, "Invalid interrupt number");

        ISRS()[InterruptNum] = std::move(function);

        const auto isc = uint8_t{1} << InterruptNum;
        EICRA = static_cast<uint8_t>(EICRA & ~((1 << isc) | (1 << (isc + 1)))) | static_cast<uint8_t>(mode << (1 << isc));
        EIMSK = EIMSK | static_cast<uint8_t>(1 << InterruptNum);
    }

    template <size_t Index>
    static void call() {
        static_assert(0 <= Index && Index < EXTERNAL_NUM_INTERRUPTS, "Invalid function index");
        return ISRS()[Index]();
    }
};

ISR (INT0_vect) { Interrupt::call<0>(); }
ISR (INT1_vect) { Interrupt::call<1>(); }