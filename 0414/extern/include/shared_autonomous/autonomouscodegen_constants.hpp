/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * Constants used in shared autonomous features.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_CONSTANTS_H_
#define AUTONOMOUSCODEGEN_CONSTANTS_H_

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
#include "autonomouscodegen_util.hpp"
#endif

namespace autonomous {
// Codegen-compatible versions of declaration in
// matlab/math/foundation/src/mfl_scalar/export/include/mfl_scalar/detail/word_types.hpp
typedef union {
    real64_T value;
    struct {
        uint32_T lw;
        uint32_T hw;
    } words;
} auton_double_word_T;

// Codegen-compatible version of declarations in
// matlab/math/foundation/src/mfl_scalar/export/include/mfl_scalar/constants.hpp
inline real64_T getInf() {
    autonomous::auton_double_word_T tmp;
    tmp.words.hw = 0x7ff00000U;
    tmp.words.lw = 0x00000000U;
    return tmp.value;
}

inline real64_T getPi() {
    return 3.14159265358979323846;
}

const real64_T tooSmall = 1e-10;
const real64_T pi = autonomous::getPi();
const real64_T twoPi = 2.0 * autonomous::getPi();
const real64_T inf = autonomous::getInf();

} // namespace autonomous

#endif // AUTONOMOUSCODEGEN_CONSTANTS_H_
