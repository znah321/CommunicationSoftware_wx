/* Copyright 2017-2019 The MathWorks, Inc. */

/**
 * @file
 * External C-API interfaces for Dubins motion primitive calculations.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C89/C90 (ANSI) standard.
 */

#ifndef AUTONOMOUSCODEGEN_DUBINS_C_API_H_
#define AUTONOMOUSCODEGEN_DUBINS_C_API_H_

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
/* To deal with the fact that PackNGo has no include file hierarchy during test */
#include "autonomouscodegen_util.hpp"
#endif


/**
 * @brief Compute segments along Dubins curve between two specified poses
 *
 * @param[in] startPose Starting pose, specified as real64_T precision
 *    matrices. Either one of @p startPose or @p goalPose can be M-by-3, while the other
 *    must be 1-by-3 or both should be M-by-3.
 * @param[in] numStartPoses Number of rows in starting pose @p startPose
 * @param[in] goalPose Ending pose. See notes for starting pose @p startPose.
 * @param[in] numGoalPoses Number of rows in ending pose @p goalPose
 * @param[in] turningRadius Turning radius (in radians)
 * @param[in] allPathTypes Boolean array indicating if path types are enabled
 *    (true) or disabled (false)
 * @param[in] numDisabledPathTypes Number of path types that are disabled.
 * @param[in] numPaths Number of total path types minus disabled path types.
 * @param[in] isOptimal TRUE if user only wants optimal path, FALSE otherwise.
 * @param[in] nlhs Number of outputs requested. The user can request 1 - 3 outputs
 * @param[out] distance Distance between @p startPose and @p goalPose, along Dubins curve.
 *    If @p startPose and @p goalPose are both 1-by-3 & paths is optimal, @p distance is a
 *    scalar. If @p startPose or @p goalPose is M-by-3, @p distance is an N-by-M
 *    matrix. where N = (6-length(disablePathType))--> (all),
 *    1-->(optimal)
 * @param[out] segmentLen Segments lengths of the path between @p startPose and @p goalPose.
 *    If @p startPose or @p goalPose is M-by-3, @p segmentLen is an 3-by-N-by-M matrix.
 *    where N = (6-@p numDisabledPathTypes)--> (all),
 *    1-->(optimal)
 * @param[out] segmentType Segments types of the path between @p startPose and @p goalPose, size is
 * same as
 *    @p segmentLen output.
 */
EXTERN_C AUTONOMOUS_CODEGEN_API void autonomousDubinsSegmentsCodegen_real64(
    const real64_T* startPose,
    const uint32_T numStartPoses,
    const real64_T* goalPose,
    const uint32_T numGoalPoses,
    const real64_T turningRadius,
    const boolean_T* allPathTypes,
    const uint32_T numDisabledPathTypes,
    const uint32_T numPaths,
    const boolean_T isOptimal,
    const uint32_T nlhs,
    real64_T* distance,
    real64_T* segmentLen,
    real64_T* segmentType);

/**
 * @brief Interpolate along shortest Dubins curve between states
 *
 * @param[in] startPose Starting pose, specified as real64_T precision 3-vector.
 * @param[in] goalPose Ending pose, specified as real64_T precision 3-vector.
 * @param[in] maxDistance Maximum distance to interpolate to, typically the range
 *   of the planner, specified as a real, double, scalar.
 * @param[in] numSteps Number of interpolation steps, specified as a real,
 *   double scalar.
 * @param[in] turningRadius Turning radius (in radians)
 * @param[out] interpPoses Interpolated poses along the Dubins curve, returned as a
 *   (2 + @p numSteps)-by-3 double-precision matrix. In addition to the
 *   @p numSteps queries, 2 transition poses are returned by
 *   @p maxDistance. These are the first 2 poses returned.
 */
EXTERN_C AUTONOMOUS_CODEGEN_API void autonomousDubinsInterpolateCodegen_real64(
    const real64_T* startPose,
    const real64_T* goalPose,
    const real64_T maxDistance,
    const uint32_T numSteps,
    const real64_T turningRadius,
    real64_T* interpPoses);

/**
 * @brief Interpolate along any Dubins curve between states
 *
 * @param[in] startPose Starting pose, specified as real64_T precision 3-vector.
 * @param[in] goalPose Ending pose, specified as real64_T precision 3-vector.
 * @param[in] samples Set of lengths at which user want to interpolate the
 *   poses of the planner, specified as a real, vector, scalar.
 * @param[in] numSamples Number of samples, specified as a real, double and scalar.
 * @param[in] turningRadius Turning radius (in radians)
 * @param[in] segmentLengths Length of the Dubins's 3 segments (in meters),
 *   specified as real64_T precision 3-vector.
 * @param[in] segmentTypes Types of the Dubins 3 segments, specified as
 *   uint32_T precision 3-vector.
 * @param[out] interpPoses Interpolated poses along the Dubins curve, returned as a
 *   (@p numSteps)-by-3 double-precision matrix.
 */
EXTERN_C AUTONOMOUS_CODEGEN_API void autonomousDubinsInterpolateSegmentsCodegen_real64(
    const real64_T* startPose,
    const real64_T* goalPose,
    const real64_T* samples,
    const uint32_T numSamples,
    const real64_T turningRadius,
    const real64_T* segmentLengths,
    const uint32_T* segmentTypes,
    real64_T* interpPoses);

/**
 * @brief Compute distance along Dubins curve between two specified poses
 *
 * @param[in] startPose Starting pose, specified as real64_T precision
 *    matrices. Either one of @p startPose or @p goalPose can be M-by-3, while the other
 *    must be 1-by-3 or both should be M-by-3.
 * @param[in] numStartPoses Number of rows in starting pose @p startPose
 * @param[in] goalPose Ending pose. See notes for starting pose @p startPose.
 * @param[in] numGoalPoses Number of rows in ending pose @p goalPose
 * @param[in] turningRadius Turning radius (in radians)
 * @param[out] distance Distance between @p startPose and @p goalPose, along Dubins curve.
 *   If @p startPose and @p goalPose are both 1-by-3, @p distance is a scalar.
 *   If @p startPose or @p goalPose is M-by-3, @p distance is an M-by-1 column vector.
 */
EXTERN_C AUTONOMOUS_CODEGEN_API void autonomousDubinsDistanceCodegen_real64(
    const real64_T* startPose,
    const uint32_T numStartPoses,
    const real64_T* goalPose,
    const uint32_T numGoalPoses,
    const real64_T turningRadius,
    real64_T* distance);

#endif /* AUTONOMOUSCODEGEN_DUBINS_C_API_H_ */
