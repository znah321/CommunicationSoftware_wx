/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * External C-API interfaces for Reeds-Shepp motion primitive calculations (TBB-acelerated)
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C89/C90 (ANSI) standard.
 */

#ifndef AUTONOMOUSCODEGEN_REEDS_TBB_API_H_
#define AUTONOMOUSCODEGEN_REEDS_TBB_API_H_

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
/* To deal with the fact that PackNGo has no include file hierarchy during test */
#include "autonomouscodegen_util.hpp"
#endif

/**
 * @copydoc autonomousReedsSheppSegmentsCodegen_real64
 *
 * This function uses TBB to parallelize the computation.
 */
EXTERN_C AUTONOMOUS_CODEGEN_API void autonomousReedsSheppSegmentsCodegen_tbb_real64(
    const real64_T* startPose,
    const uint32_T numStartPoses,
    const real64_T* goalPose,
    const uint32_T numGoalPoses,
    const real64_T turningRadius,
    const real64_T forwardCost,
    const real64_T reverseCost,
    const boolean_T* allPathTypes,
    const uint32_T numDisabledPathTypes,
    const uint32_T numPaths,
    const boolean_T isOptimal,
    const uint32_T nlhs,
    real64_T* distance,
    real64_T* segmentLen,
    real64_T* segmentType);


/**
 * @copydoc autonomousReedsSheppDistanceCodegen_real64
 *
 * This function uses TBB to parallelize the computation.
 */
EXTERN_C AUTONOMOUS_CODEGEN_API void autonomousReedsSheppDistanceCodegen_tbb_real64(
    const real64_T* startPose,
    const uint32_T numStartPoses,
    const real64_T* goalPose,
    const uint32_T numGoalPoses,
    const real64_T turningRadius,
    const real64_T reverseCost,
    real64_T* distance);

#endif /* AUTONOMOUSCODEGEN_REEDS_TBB_API_H_ */
