/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * Trigonometric utility functions for all autonomous functionality in codegen.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_TRIG_H_
#define AUTONOMOUSCODEGEN_TRIG_H_

#include <cmath> // for std::fabs

#include <math.h> // for fmod, sqrt, floor, atan2

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_constants.hpp" // for pi, twoPi, tooSmall
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
#include "autonomouscodegen_constants.hpp"
#include "autonomouscodegen_util.hpp"
#endif

/** @file
 * Declarations for trigonometric operations.
 */

namespace autonomous {

/// Wrap angle to interval [0, 2pi]
/**
 * @param angleRad Input angle (in radians)
 * @return Angle wrapped to interval [0, 2pi] (in radians)
 */
inline real64_T wrapToTwoPi(const real64_T angleRad) {
    // Deal with very small negative angles.
    if (angleRad < 0 && angleRad > -tooSmall) {
        return 0;
    }

    return angleRad - twoPi * floor(angleRad / twoPi);
}

/// Wrap angle to interval [-pi, pi]
/**
 * @param[in] angleRad Input angle (in radians)
 * @return Angle wrapped to interval [-pi, pi] (in radians)
 */
inline real64_T wrapToPi(const real64_T angleRad) {
    real64_T wrappedAngle = fmod(angleRad, twoPi);
    if (wrappedAngle < -pi) {
        wrappedAngle += twoPi;
    } else if (wrappedAngle > pi) {
        wrappedAngle -= twoPi;
    }

    return wrappedAngle;
}

/// Convert Cartesian (x,y) to polar (r,theta) coordinates
/**
 * @param[in] x X coordinate in Cartesian frame
 * @param[in] y Y coordinate in Cartesian frame
 * @param[out] r Radius of polar coordinate
 * @param[out] theta Angle of polar coordinate (azimuth)
 */
inline void toPolar(const real64_T x, const real64_T y, real64_T& r, real64_T& theta) {
    r = sqrt(x * x + y * y);
    theta = atan2(y, x);
}

// Codegen-compatible version of declarations in
// matlab/math/foundation/src/mfl_scalar/export/include/mfl_scalar/basic_math.hpp
/// Calculate absolute value of input
/**
 * This is a codegen-compatible version of code in
 * matlab/math/foundation/src/mfl_scalar/export/include/mfl_scalar/basic_math.hpp
 *
 * @param[in] x Input value
 * @return Absolute value of input
 */
inline real64_T abs(const real64_T x) {
    return std::fabs(x);
}

} // namespace autonomous

#endif // AUTONOMOUSCODEGEN_TRIG_H_
