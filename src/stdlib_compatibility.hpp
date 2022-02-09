#pragma once
#include <cassert>

namespace std {
    struct nothrow_t { explicit nothrow_t() = default; };
    [[maybe_unused]] const nothrow_t nothrow;

    inline void __throw_bad_function_call() { assert(false && "Bad function call"); }
}

#define _LOCALE_FACETS_H 0

