#pragma once

#include <catch2/catch_test_macros.hpp>

// RED phase (default): every guarded test fails until UC11_GREEN_PHASE is enabled.
inline void uc11_require_green_phase() {
#if !defined(UC11_GREEN_PHASE)
    FAIL("RED");
#endif
}
