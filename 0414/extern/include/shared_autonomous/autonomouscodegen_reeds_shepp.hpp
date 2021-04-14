/* Copyright 2017-2018 The MathWorks, Inc. */

/**
 * @file
 * Main interfaces for Reeds-Shepp motion primitive calculations.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_REEDSSHEPP_H_
#define AUTONOMOUSCODEGEN_REEDSSHEPP_H_

#include <math.h>    // for sqrt, trigonometric functions
#include <algorithm> // for max
#include <vector>

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_reeds_shepp_primitives.hpp"
#else
// To deal with the fact that PackNGo has no include file hierarchy during test
#include "autonomouscodegen_reeds_shepp_primitives.hpp"
#endif

namespace autonomous {

namespace reedsshepp {
/*
 * The code for ReedsShepp primitives and computation of the various
 * segment paths is inspired from Open Motion Planning Library source.
 */

/*
 * RSSegmentType - Enumeration for holding segment type
 */
enum RSSegmentType { Left = 0, Right, Straight, NOP };

const RSSegmentType pathToSegment[18][5] = {
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Right, autonomous::reedsshepp::Left,
     autonomous::reedsshepp::NOP, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Left, autonomous::reedsshepp::Right,
     autonomous::reedsshepp::NOP, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Right, autonomous::reedsshepp::Left,
     autonomous::reedsshepp::Right, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Left, autonomous::reedsshepp::Right,
     autonomous::reedsshepp::Left, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Right, autonomous::reedsshepp::Straight,
     autonomous::reedsshepp::Left, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Left, autonomous::reedsshepp::Straight,
     autonomous::reedsshepp::Right, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Straight, autonomous::reedsshepp::Right,
     autonomous::reedsshepp::Left, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Straight, autonomous::reedsshepp::Left,
     autonomous::reedsshepp::Right, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Right, autonomous::reedsshepp::Straight,
     autonomous::reedsshepp::Right, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Left, autonomous::reedsshepp::Straight,
     autonomous::reedsshepp::Left, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Straight, autonomous::reedsshepp::Right,
     autonomous::reedsshepp::Left, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Straight, autonomous::reedsshepp::Left,
     autonomous::reedsshepp::Right, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Straight, autonomous::reedsshepp::Right,
     autonomous::reedsshepp::NOP, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Straight, autonomous::reedsshepp::Left,
     autonomous::reedsshepp::NOP, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Straight, autonomous::reedsshepp::Left,
     autonomous::reedsshepp::NOP, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Straight, autonomous::reedsshepp::Right,
     autonomous::reedsshepp::NOP, autonomous::reedsshepp::NOP},
    {autonomous::reedsshepp::Left, autonomous::reedsshepp::Right, autonomous::reedsshepp::Straight,
     autonomous::reedsshepp::Left, autonomous::reedsshepp::Right},
    {autonomous::reedsshepp::Right, autonomous::reedsshepp::Left, autonomous::reedsshepp::Straight,
     autonomous::reedsshepp::Right, autonomous::reedsshepp::Left}};

/*
 * shortestReedsSheppPath returns shortest normalized Reeds-Shepp path.
 */
inline autonomous::reedsshepp::ReedsSheppPath shortestReedsSheppPath(real64_T x,
                                                                     real64_T y,
                                                                     real64_T phi,
                                                                     real64_T reverseCost) {
    autonomous::reedsshepp::ReedsSheppPath minPath, tempPath;
    real64_T minLength, tempLength;

    autonomous::reedsshepp::computeCSCPath(x, y, phi, reverseCost, minPath);
    minLength = minPath.length();

    autonomous::reedsshepp::computeCCCPath(x, y, phi, reverseCost, tempPath);
    tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    autonomous::reedsshepp::computeCCCCPath(x, y, phi, reverseCost, tempPath);
    tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    autonomous::reedsshepp::computeCCSCPath(x, y, phi, reverseCost, tempPath);
    tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    autonomous::reedsshepp::computeCCSCCPath(x, y, phi, reverseCost, tempPath);
    tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    return minPath;
}

/*
 * computeReedsSheppPath - compute Reeds-Shepp shortest/all path.
 */
inline autonomous::reedsshepp::ReedsSheppPath computeReedsSheppPath(
    const real64_T initialConfiguration[3],
    const real64_T finalConfiguration[3],
    const real64_T turningRadius,
    const real64_T reverseCost) {
    real64_T x1 = initialConfiguration[0];
    real64_T y1 = initialConfiguration[1];
    real64_T theta1 = initialConfiguration[2];
    real64_T x2 = finalConfiguration[0];
    real64_T y2 = finalConfiguration[1];
    real64_T theta2 = finalConfiguration[2];

    real64_T dx = x2 - x1;
    real64_T dy = y2 - y1;
    real64_T c = cos(theta1);
    real64_T s = sin(theta1);

    real64_T x = c * dx + s * dy;
    real64_T y = -s * dx + c * dy;
    real64_T phi = theta2 - theta1;

    return shortestReedsSheppPath(x / turningRadius, y / turningRadius, phi, reverseCost);
}

/*
 * AllReedsSheppPath returns all normalized Reeds-Shepp path.
 */
inline std::vector<autonomous::reedsshepp::ReedsSheppPath> AllReedsSheppPath(
    real64_T x,
    real64_T y,
    real64_T phi,
    real64_T forwardCost,
    real64_T reverseCost,
    const boolean_T isOptimal,
    const boolean_T allPathTypes[autonomous::reedsshepp::TotalNumPaths],
    const uint32_T numTotalPathTypes) {
    std::vector<autonomous::reedsshepp::ReedsSheppPath> allPaths;
    allPaths.reserve(numTotalPathTypes);

    autonomous::reedsshepp::computeCSCPathAll(x, y, phi, forwardCost, reverseCost, &allPathTypes[0],
                                              &allPaths);
    autonomous::reedsshepp::computeCCCPathAll(x, y, phi, forwardCost, reverseCost, &allPathTypes[0],
                                              &allPaths);
    autonomous::reedsshepp::computeCCCCPathAll(x, y, phi, forwardCost, reverseCost,
                                               &allPathTypes[0], &allPaths);
    autonomous::reedsshepp::computeCCSCPathAll(x, y, phi, forwardCost, reverseCost,
                                               &allPathTypes[0], &allPaths);
    autonomous::reedsshepp::computeCCSCCPathAll(x, y, phi, forwardCost, reverseCost,
                                                &allPathTypes[0], &allPaths);

    if (isOptimal) {
        real64_T optimalPath = autonomous::inf;
        uint32_T optimalInd = 0;

        // Round up to 15 decimal point to get same optimal path in
        // all system (maci, linux, windows). (Geck --> g1770360)
        real64_T round_val = pow(10.0, 15);
        for (uint32_T ind = 0; ind < numTotalPathTypes; ++ind) {
            // Round up to 15 decimal point to get same optimal path in
            // all system (maci, linux, windows . (Geck --> g1770360)
            real64_T tempcost = round(allPaths[ind].cost() * round_val) / round_val;
            if (optimalPath > tempcost) {
                optimalPath = tempcost;
                optimalInd = ind;
            }
        }

        std::vector<autonomous::reedsshepp::ReedsSheppPath> path(1);
        path[0] = allPaths[optimalInd];

        return path;
    }
    return allPaths;
}

/*
 * computeAllReedsSheppPaths - compute Reeds-Shepp shortest/all path.
 */
inline std::vector<autonomous::reedsshepp::ReedsSheppPath> computeAllReedsSheppPaths(
    const real64_T initialConfiguration[3],
    const real64_T finalConfiguration[3],
    const real64_T turningRadius,
    const real64_T forwardCost,
    const real64_T reverseCost,
    const boolean_T isOptimal,
    const boolean_T allPathTypes[autonomous::reedsshepp::TotalNumPaths],
    const uint32_T numTotalPathTypes) {
    real64_T x1 = initialConfiguration[0];
    real64_T y1 = initialConfiguration[1];
    real64_T theta1 = initialConfiguration[2];
    real64_T x2 = finalConfiguration[0];
    real64_T y2 = finalConfiguration[1];
    real64_T theta2 = finalConfiguration[2];

    real64_T dx = x2 - x1;
    real64_T dy = y2 - y1;
    real64_T c = cos(theta1);
    real64_T s = sin(theta1);

    real64_T x = c * dx + s * dy;
    real64_T y = -s * dx + c * dy;
    real64_T phi = theta2 - theta1;

    return AllReedsSheppPath(x / turningRadius, y / turningRadius, phi, forwardCost, reverseCost,
                             isOptimal, &allPathTypes[0], numTotalPathTypes);
}

/*
 * interpolateAlongInitializedRSPath - interpolate points along a
 * pre-computed ReedsShepp path.
 */
inline void interpolateAlongInitializedRSPath(const real64_T pathLength,
                                              const RSSegmentType* segments,
                                              const real64_T* segmentLengths,
                                              const real64_T* from,
                                              const real64_T* towards,
                                              const real64_T t,
                                              const real64_T turningRadius,
                                              real64_T* state) {
    if (t <= 0.0) {
        state[0] = from[0];
        state[1] = from[1];
        state[2] = from[2];
    } else if (t >= 1.0) {
        state[0] = towards[0];
        state[1] = towards[1];
        state[2] = towards[2];
    } else {

        // Initialize at [0, 0, theta]
        state[0] = 0;
        state[1] = 0;
        state[2] = from[2];

        real64_T seg = t * pathLength;
        real64_T v, phi;

        // Compute normalized update.
        for (uint32_T i = 0; i < 5 && seg > 0; ++i) {
            if (segmentLengths[i] < 0) {
                v = std::max(-seg, segmentLengths[i]);
                seg += v;
            } else {
                v = std::min(seg, segmentLengths[i]);
                seg -= v;
            }

            phi = state[2];

            switch (segments[i]) {
            case autonomous::reedsshepp::Left:
                state[0] += sin(phi + v) - sin(phi);
                state[1] += -cos(phi + v) + cos(phi);
                state[2] = autonomous::wrapToTwoPi(phi + v);
                break;

            case autonomous::reedsshepp::Right:
                state[0] += -sin(phi - v) + sin(phi);
                state[1] += cos(phi - v) - cos(phi);
                state[2] = autonomous::wrapToTwoPi(phi - v);
                break;

            case autonomous::reedsshepp::Straight:
                state[0] += v * cos(phi);
                state[1] += v * sin(phi);
                break;

            case autonomous::reedsshepp::NOP:
                // no update needed
                break;
            }
        }
        // Denormalize and update.
        state[0] = from[0] + state[0] * turningRadius;
        state[1] = from[1] + state[1] * turningRadius;
    }
}

/*
 * interpolateRS - interpolate alone ReedsShepp curve between states.
 */
inline void interpolateRS(const real64_T* from,
                          const real64_T* towards,
                          const real64_T maxDistance,
                          const uint32_T numSteps,
                          const real64_T turningRadius,
                          const real64_T reverseCost,
                          real64_T* state) {
    // Compute ReedsShepp path.
    autonomous::reedsshepp::ReedsSheppPath path =
        computeReedsSheppPath(from, towards, turningRadius, reverseCost);

    const RSSegmentType* segments = pathToSegment[path.getPathType()];

    // Compute the fraction of path to be traversed based on maximum
    // connection distance (maxDistance).
    //  * If the distance between the states is less than maxDistance, we
    //    interpolate all the through to the destination state (towards).
    //  * If the distance between the states is more than maxDistance, we
    //    only interpolate a fraction of the path between the two states.

    // Find the distance between the states.
    const real64_T pathLength = path.length();
    const real64_T dist = pathLength * turningRadius;

    // Find the fraction of the path to interpolate.
    const real64_T t = std::min(maxDistance / dist, 1.0);

    // Find interpolation step based on number of steps.
    const real64_T step = t / static_cast<real64_T>(numSteps);

    std::vector<real64_T> temp(3, 0);
    real64_T fraction = 0;

    // Interpolate along transition points
    const uint32_T numTransitions = 4;
    const uint32_T arrLength = numSteps + numTransitions;

    const real64_T* segLengths = path.getSegmentLengths();

    for (uint32_T n = 0; n < numTransitions; ++n) {
        // Compute fraction along path corresponding to n-th transition
        // point
        fraction += (autonomous::abs(segLengths[n]) / pathLength);

        // Saturate at maxDistance. This has the effect of returning
        // the ending pose for all transition poses that come after the
        // maxDistance has been reached.
        fraction = std::min(fraction, t);

        interpolateAlongInitializedRSPath(pathLength, segments, segLengths, from, towards, fraction,
                                          turningRadius, &temp[0]);

        state[n] = temp[0];
        state[n + arrLength] = temp[1];
        state[n + 2 * arrLength] = temp[2];
    }

    // Interpolate along path at equidistant intervals
    fraction = 0;
    for (uint32_T n = numTransitions; n < arrLength; ++n) {
        fraction += step;
        interpolateAlongInitializedRSPath(pathLength, segments, segLengths, from, towards, fraction,
                                          turningRadius, &temp[0]);

        state[n] = temp[0];
        state[n + arrLength] = temp[1];
        state[n + 2 * arrLength] = temp[2];
    }
}

} // namespace reedsshepp
} // namespace autonomous

#endif /* AUTONOMOUSCODEGEN_REEDSSHEPP_H_ */
