/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * Constants used for Reeds-Shepp motion primitive calculations.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_REEDS_SHEPP_CONSTANTS_H_
#define AUTONOMOUSCODEGEN_REEDS_SHEPP_CONSTANTS_H_

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
#include "autonomouscodegen_util.hpp"
#endif

namespace autonomous {
namespace reedsshepp {

static const uint32_T TotalNumPaths = 44; ///< Total number of possible Reeds-Shepp paths

} // namespace reedsshepp
} // namespace autonomous

#endif // AUTONOMOUSCODEGEN_REEDS_SHEPP_CONSTANTS_H_
