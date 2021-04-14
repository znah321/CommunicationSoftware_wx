/* Copyright 2017-2019 The MathWorks, Inc. */

/**
 * @file
 * Main interfaces for Dubins motion primitive calculations.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_DUBINS_HPP_
#define AUTONOMOUSCODEGEN_DUBINS_HPP_

#include <math.h>    // for sqrt, trigonometric functions
#include <algorithm> // for max
#include <string>
#include <vector>

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_constants.hpp"        // for inf, tooSmall
#include "autonomouscodegen/autonomouscodegen_dubins_constants.hpp" // for TotalNumPaths
#include "autonomouscodegen/autonomouscodegen_trig.hpp"             // for wrapToTwoPi
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
// To deal with the fact that PackNGo has no include file hierarchy during test
#include "autonomouscodegen_constants.hpp"
#include "autonomouscodegen_dubins_constants.hpp"
#include "autonomouscodegen_trig.hpp"
#include "autonomouscodegen_util.hpp"
#endif

namespace autonomous {
namespace dubins {

/*
 * DubinsPathType - Enumeration for holding path type.
 */
enum DubinsPathType { LSL = 0, LSR, RSL, RSR, RLR, LRL };

/*
 * DubinsSegmentType - Enumeration for holding segment type.
 */
enum DubinsSegmentType { Left, Right, Straight };

const DubinsSegmentType pathToSegment[dubins::TotalNumPaths][3] = {
    {autonomous::dubins::Left, autonomous::dubins::Straight, autonomous::dubins::Left},
    {autonomous::dubins::Left, autonomous::dubins::Straight, autonomous::dubins::Right},
    {autonomous::dubins::Right, autonomous::dubins::Straight, autonomous::dubins::Left},
    {autonomous::dubins::Right, autonomous::dubins::Straight, autonomous::dubins::Right},
    {autonomous::dubins::Right, autonomous::dubins::Left, autonomous::dubins::Right},
    {autonomous::dubins::Left, autonomous::dubins::Right, autonomous::dubins::Left}};

/*
 * DubinsPath - Encapsulate normalized Dubins path.
 */
class DubinsPath {

  public:
    DubinsPath(const real64_T firstSegment = 0.,
               const real64_T secondSegment = 0.,
               const real64_T thirdSegment = 0.,
               const DubinsPathType pathType = autonomous::dubins::LSL)
        : pathType_(pathType) {

        segmentLengths_[0] = firstSegment;
        segmentLengths_[1] = secondSegment;
        segmentLengths_[2] = thirdSegment;
    }

    real64_T length() const {
        return segmentLengths_[0] + segmentLengths_[1] + segmentLengths_[2];
    }

    DubinsPathType getPathType() const {
        return pathType_;
    }

    const real64_T* getSegmentLengths() const {

        return &segmentLengths_[0];
    }

  private:
    DubinsPathType pathType_;
    real64_T segmentLengths_[3];
};

struct FindShortestDubinsPath {
    bool operator()(DubinsPath p1, DubinsPath p2) {
        return p1.length() < p2.length();
    }
};

/*
 * dubinsLSL - compute DubinsPath for LSL path.
 */
inline DubinsPath dubinsLSL(const real64_T d, const real64_T alpha, const real64_T beta) {
    real64_T ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);
    real64_T tmp = 2. + d * d - 2. * (ca * cb + sa * sb - d * (sa - sb));
    if (tmp >= -tooSmall) {
        real64_T theta = atan2(cb - ca, d + sa - sb);
        real64_T t = wrapToTwoPi(-alpha + theta);
        real64_T p = sqrt(std::max(tmp, 0.));
        real64_T q = wrapToTwoPi(beta - theta);
        return DubinsPath(t, p, q, autonomous::dubins::LSL);
    }
    return DubinsPath(0, autonomous::inf, 0, autonomous::dubins::LSL);
}

/*
 * dubinsLSR - compute DubinsPath for LSR path.
 */
inline DubinsPath dubinsLSR(const real64_T d, const real64_T alpha, const real64_T beta) {
    real64_T ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);
    real64_T tmp = -2. + d * d + 2. * (ca * cb + sa * sb + d * (sa + sb));
    if (tmp >= -tooSmall) {
        real64_T p = sqrt(std::max(tmp, 0.));
        real64_T theta = atan2(-ca - cb, d + sa + sb) - atan2(-2., p);
        real64_T t = wrapToTwoPi(-alpha + theta);
        real64_T q = wrapToTwoPi(-beta + theta);
        return DubinsPath(t, p, q, autonomous::dubins::LSR);
    }
    return DubinsPath(0, autonomous::inf, 0, autonomous::dubins::LSR);
}

/*
 * dubinsRSL - compute DubinsPath for RSL path.
 */
inline DubinsPath dubinsRSL(const real64_T d, const real64_T alpha, const real64_T beta) {
    real64_T ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);
    real64_T tmp = d * d - 2. + 2. * (ca * cb + sa * sb - d * (sa + sb));
    if (tmp >= -tooSmall) {
        real64_T p = sqrt(std::max(tmp, 0.));
        real64_T theta = atan2(ca + cb, d - sa - sb) - atan2(2., p);
        real64_T t = wrapToTwoPi(alpha - theta);
        real64_T q = wrapToTwoPi(beta - theta);
        return DubinsPath(t, p, q, autonomous::dubins::RSL);
    }
    return DubinsPath(0, autonomous::inf, 0, autonomous::dubins::RSL);
}

/*
 * dubinsRSR - compute DubinsPath for RSR path.
 */
inline DubinsPath dubinsRSR(const real64_T d, const real64_T alpha, const real64_T beta) {
    real64_T ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);
    real64_T tmp = 2. + d * d - 2. * (ca * cb + sa * sb - d * (sb - sa));
    if (tmp >= -tooSmall) {
        real64_T theta = atan2(ca - cb, d - sa + sb);
        real64_T t = wrapToTwoPi(alpha - theta);
        real64_T p = sqrt(std::max(tmp, 0.));
        real64_T q = wrapToTwoPi(-beta + theta);
        return DubinsPath(t, p, q, autonomous::dubins::RSR);
    }
    return DubinsPath(0, autonomous::inf, 0, autonomous::dubins::RSR);
}

/*
 * dubinsRLR - compute DubinsPath for RLR path.
 */
inline DubinsPath dubinsRLR(const real64_T d, const real64_T alpha, const real64_T beta) {
    real64_T ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);
    real64_T tmp = .125 * (6. - d * d + 2. * (ca * cb + sa * sb + d * (sa - sb)));
    if (fabs(tmp) < 1.) {
        real64_T p = twoPi - acos(tmp);
        real64_T theta = atan2(ca - cb, d - sa + sb);
        real64_T t = wrapToTwoPi(alpha - theta + .5 * p);
        real64_T q = wrapToTwoPi(alpha - beta - t + p);
        return DubinsPath(t, p, q, autonomous::dubins::RLR);
    }
    return DubinsPath(0, autonomous::inf, 0, autonomous::dubins::RLR);
}


/*
 * dubinsLRL - compute DubinsPath for LRL path.
 */
inline DubinsPath dubinsLRL(const real64_T d, const real64_T alpha, const real64_T beta) {
    real64_T ca = cos(alpha), sa = sin(alpha), cb = cos(beta), sb = sin(beta);
    real64_T tmp = .125 * (6. - d * d + 2. * (ca * cb + sa * sb - d * (sa - sb)));
    if (fabs(tmp) < 1.) {
        real64_T p = twoPi - acos(tmp);
        real64_T theta = atan2(-ca + cb, d + sa - sb);
        real64_T t = wrapToTwoPi(-alpha + theta + .5 * p);
        real64_T q = wrapToTwoPi(beta - alpha - t + p);
        return DubinsPath(t, p, q, autonomous::dubins::LRL);
    }
    return DubinsPath(0, autonomous::inf, 0, autonomous::dubins::LRL);
}

/*
 * shortestDubins return shortest normalized Dubins path.
 */
inline DubinsPath shortestDubins(const real64_T d, const real64_T alpha, const real64_T beta) {
    DubinsPath path;

    if (d < tooSmall && fabs(alpha - beta) < tooSmall) {
        return DubinsPath(0, d, 0);
    }

    DubinsPath minPath = dubinsLSL(d, alpha, beta);
    real64_T minLength = minPath.length();

    DubinsPath tempPath = dubinsLSR(d, alpha, beta);
    real64_T tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    tempPath = dubinsRSL(d, alpha, beta);
    tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    tempPath = dubinsRSR(d, alpha, beta);
    tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    tempPath = dubinsRLR(d, alpha, beta);
    tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    tempPath = dubinsLRL(d, alpha, beta);
    tempLength = tempPath.length();

    if (tempLength < minLength) {
        minLength = tempLength;
        minPath = tempPath;
    }

    return minPath;
}

/*
 * computeDubinsPath - compute Dubins shortest/all path.
 */

inline DubinsPath computeDubinsPath(const real64_T startPose[3],
                                    const real64_T goalPose[3],
                                    const real64_T turningRadius) {
    const real64_T dx = goalPose[0] - startPose[0];
    const real64_T dy = goalPose[1] - startPose[1];
    const real64_T d = sqrt(dx * dx + dy * dy) / turningRadius;

    const real64_T theta = atan2(dy, dx);

    const real64_T alpha = wrapToTwoPi(startPose[2] - theta);
    const real64_T beta = wrapToTwoPi(goalPose[2] - theta);

    return shortestDubins(d, alpha, beta);
}

/*
 * allDubins return shortest/all normalized Dubins path.
 */
inline std::vector<DubinsPath> allDubins(const real64_T d,
                                         const real64_T alpha,
                                         const real64_T beta,
                                         const boolean_T isOptimal,
                                         const boolean_T allPathTypes[dubins::TotalNumPaths],
                                         const uint32_T numTotalPathTypes) {
    std::vector<DubinsPath> allPaths(numTotalPathTypes);
    uint32_T i = 0;

    // Handling empty paths
    if (d < tooSmall && fabs(alpha - beta) < tooSmall) {
        static const real64_T arr[] = {d, d, d, d, autonomous::inf, autonomous::inf};
        std::vector<real64_T> cost(arr, arr + sizeof(arr) / sizeof(arr[0]));
        DubinsPathType allDubinsPathType[dubins::TotalNumPaths] = {
            autonomous::dubins::LSL, autonomous::dubins::LSR, autonomous::dubins::RSL,
            autonomous::dubins::RSR, autonomous::dubins::RLR, autonomous::dubins::LRL};

        for (uint32_T j = 0; j < dubins::TotalNumPaths; ++j) {
            if (allPathTypes[j]) {
                allPaths[i++] = DubinsPath(0, cost[j], 0, allDubinsPathType[j]);
            }
        }
    } else {
        if (allPathTypes[0]) {
            allPaths[i++] = dubinsLSL(d, alpha, beta);
        }

        if (allPathTypes[1]) {
            allPaths[i++] = dubinsLSR(d, alpha, beta);
        }

        if (allPathTypes[2]) {
            allPaths[i++] = dubinsRSL(d, alpha, beta);
        }

        if (allPathTypes[3]) {
            allPaths[i++] = dubinsRSR(d, alpha, beta);
        }

        if (allPathTypes[4]) {
            allPaths[i++] = dubinsRLR(d, alpha, beta);
        }

        if (allPathTypes[5]) {
            allPaths[i++] = dubinsLRL(d, alpha, beta);
        }
    }

    if (isOptimal) {
        // auto findShortest = [](DubinsPath p1, DubinsPath p2) { return p1.length() < p2.length();
        // };
        struct FindShortestDubinsPath findShortest;

        // Find the shortest path and place it in a one-element vector
        std::vector<DubinsPath> singlePath;
        singlePath.push_back(*(std::min_element(allPaths.begin(), allPaths.end(), findShortest)));
        return singlePath;
        // return {*(std::min_element(allPaths.begin(), allPaths.end(), findShortest))};
    }

    return allPaths;
}

/*
 * computeAllDubinsPaths - compute Dubins shortest/all path.
 */

inline std::vector<DubinsPath> computeAllDubinsPaths(
    const real64_T startPose[3],
    const real64_T goalPose[3],
    const real64_T turningRadius,
    const boolean_T isOptimal,
    const boolean_T allPathTypes[dubins::TotalNumPaths],
    const uint32_T numTotalPathTypes) {
    const real64_T dx = goalPose[0] - startPose[0];
    const real64_T dy = goalPose[1] - startPose[1];
    const real64_T d = sqrt(dx * dx + dy * dy) / turningRadius;

    const real64_T theta = atan2(dy, dx);

    const real64_T alpha = wrapToTwoPi(startPose[2] - theta);
    const real64_T beta = wrapToTwoPi(goalPose[2] - theta);

    return allDubins(d, alpha, beta, isOptimal, &allPathTypes[0], numTotalPathTypes);
}

/*
 * interpolateAlongInitializedDPath - interpolate points along a
 * pre-computed Dubins path.
 */
inline void interpolateAlongInitializedDPath(const real64_T pathLength,
                                             const DubinsSegmentType* segments,
                                             const real_T* segmentLengths,
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
        // Initialize at [0,0,theta]
        state[0] = 0;
        state[1] = 0;
        state[2] = from[2];

        real64_T seg = t * pathLength;
        real64_T v, phi;

        // Compute normalized update.
        for (uint32_T i = 0; i < 3 && seg > 0; ++i) {
            v = std::min(seg, segmentLengths[i]);
            phi = state[2];

            seg -= v;

            switch (segments[i]) {
            case autonomous::dubins::Left:
                state[0] += sin(phi + v) - sin(phi);
                state[1] += -cos(phi + v) + cos(phi);
                state[2] = wrapToTwoPi(phi + v);
                break;

            case autonomous::dubins::Right:
                state[0] += -sin(phi - v) + sin(phi);
                state[1] += cos(phi - v) - cos(phi);
                state[2] = wrapToTwoPi(phi - v);
                break;

            case autonomous::dubins::Straight:
                state[0] += v * cos(phi);
                state[1] += v * sin(phi);
                break;
            }
        }
        // Denormalize and update.
        state[0] = from[0] + state[0] * turningRadius;
        state[1] = from[1] + state[1] * turningRadius;
    }
}

/*
 * interpolateDubins - interpolate along Dubins curve between states.
 */
inline void interpolateDubins(const real64_T* from,
                              const real64_T* towards,
                              const real64_T maxDistance,
                              const uint32_T numSteps,
                              const real64_T turningRadius,
                              real64_T* state) {
    // Compute Dubins path.
    DubinsPath path = computeDubinsPath(from, towards, turningRadius);

    const DubinsSegmentType* segments = pathToSegment[path.getPathType()];

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

    real64_T temp[3] = {0, 0, 0};
    real64_T fraction = 0;

    // Interpolate along transition points
    const uint32_T numTransitions = 2;
    const uint32_T arrLength = numSteps + numTransitions;

    const real64_T* segLengths = path.getSegmentLengths();

    for (uint32_T n = 0; n < numTransitions; ++n) {
        // Compute fraction along path corresponding to n-th transition
        // point
        fraction += (segLengths[n] / pathLength);

        // Saturate at maxDistance. This has the effect of returning
        // the ending pose for all transition poses that come after the
        // maxDistance has been reached.
        fraction = std::min(fraction, t);

        interpolateAlongInitializedDPath(pathLength, segments, segLengths, from, towards, fraction,
                                         turningRadius, &temp[0]);

        state[n] = temp[0];
        state[n + arrLength] = temp[1];
        state[n + 2 * arrLength] = temp[2];
    }

    // Interpolate along path at equidistant intervals
    fraction = 0;
    for (uint32_T n = numTransitions; n < arrLength; ++n) {
        fraction += step;
        interpolateAlongInitializedDPath(pathLength, segments, segLengths, from, towards, fraction,
                                         turningRadius, &temp[0]);

        state[n] = temp[0];
        state[n + arrLength] = temp[1];
        state[n + 2 * arrLength] = temp[2];
    }
}

/*
 * interpolateDubins - interpolate along given states and Dubins curve.
 */
inline void interpolateDubinsSegments(const real64_T* from,
                                      const real64_T* towards,
                                      const real64_T* samples,
                                      const uint32_T numSamples,
                                      const real64_T turningRadius,
                                      const real64_T* segmentLengths,
                                      const uint32_T* segmentTypes,
                                      real64_T* state) {
    DubinsSegmentType segments[3] = {static_cast<DubinsSegmentType>(segmentTypes[0]),
                                     static_cast<DubinsSegmentType>(segmentTypes[1]),
                                     static_cast<DubinsSegmentType>(segmentTypes[2])};

    real64_T temp[3] = {0, 0, 0};        

    const real64_T pathLength = segmentLengths[0] + segmentLengths[1] + segmentLengths[2];

    real64_T normalizedSegmentLengths[3] = {segmentLengths[0] / turningRadius,
                                            segmentLengths[1] / turningRadius,
                                            segmentLengths[2] / turningRadius};                                            
    
    const real64_T normalizedPathLength =
        normalizedSegmentLengths[0] + normalizedSegmentLengths[1] + normalizedSegmentLengths[2];

    for (uint32_T n = 0; n < numSamples; ++n) {
        // Compute fraction along path corresponding to n-th transition
        // point
        interpolateAlongInitializedDPath(normalizedPathLength, segments, normalizedSegmentLengths,
                                         from, towards, samples[n] / pathLength, turningRadius,
                                         &temp[0]);

        state[n]                    = temp[0];
        state[n + numSamples]       = temp[1];
        state[n + 2 * numSamples]   = temp[2];
    }
}
} // namespace dubins
} // namespace autonomous

#endif /* AUTONOMOUSCODEGEN_DUBINS_HPP_ */
