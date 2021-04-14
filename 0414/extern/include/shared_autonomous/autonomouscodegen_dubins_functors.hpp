/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * Functions used for Dubins motion primitive calculations.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_DUBINS_FUNCTORS_HPP_
#define AUTONOMOUSCODEGEN_DUBINS_FUNCTORS_HPP_

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_constants.hpp"
#include "autonomouscodegen/autonomouscodegen_dubins.hpp"
#include "autonomouscodegen/autonomouscodegen_parallel_range.hpp"
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
// To deal with the fact that PackNGo has no include file hierarchy during test
#include "autonomouscodegen_constants.hpp"
#include "autonomouscodegen_dubins.hpp"
#include "autonomouscodegen_parallel_range.hpp"
#include "autonomouscodegen_util.hpp"
#endif

namespace autonomous {
namespace dubins {

/**
 * @brief Add extracted (path cost, path segments, path types) to output vector.
 */
template <typename T>
void addCostSegLensSegTypes(T* distances,
                            T* segmentLen,
                            T* segmentType,
                            std::vector<DubinsPath> path,
                            const uint32_T m,
                            const uint32_T numPaths,
                            const T turningRadius,
                            const uint32_T numInputs) {
    // Add cost
    for (uint32_T n = 0; n < numPaths; ++n) {
        distances[n + m * numPaths] = path[n].length() * turningRadius;
    }

    // Add segment lengths
    if (numInputs == 2) {
        for (uint32_T n = 0; n < numPaths; ++n) {
            const T* segLens = path[n].getSegmentLengths();
            segmentLen[0 + n * 3 + m * 3 * numPaths] = segLens[0] * turningRadius;
            segmentLen[1 + n * 3 + m * 3 * numPaths] = segLens[1] * turningRadius;
            segmentLen[2 + n * 3 + m * 3 * numPaths] = segLens[2] * turningRadius;
        }
    }

    // Add segment lengths & segment type
    if (numInputs == 3) {
        for (uint32_T n = 0; n < numPaths; ++n) {
            const T* segLens = path[n].getSegmentLengths();
            for (uint32_T k = 0; k < static_cast<uint32_T>(3); ++k) {
                segmentLen[k + n * 3 + m * 3 * numPaths] = segLens[k] * turningRadius;
                segmentType[k + n * 3 + m * 3 * numPaths] =
                    static_cast<int32_T>(pathToSegment[path[n].getPathType()][k]);
            }
        }
    }
}

/**
 * @brief Functor for evaluating Dubins motion primitive segments
 *
 * This function is used to allow both parallel (with TBB) and serial execution.
 * @see autonomousDubinsSegmentsCodegen_real64, autonomousDubinsSegmentsCodegen_tbb_real64
 */
template <typename T>
class DubinsSegmentsFunctor {
  private:
    const T* m_startPose;
    const uint32_T m_maxNumPoses;
    const uint32_T m_numStartPoses;
    const uint32_T m_numGoalPoses;
    const T* m_goalPose;
    const T m_turningRadius;
    const boolean_T* m_allPathTypes;
    const uint32_T m_numTotalPathTypes;
    const uint32_T m_numPaths;
    const boolean_T m_isOptimal;
    const uint32_T m_nlhs;
    T* m_distance;
    T* m_segmentLen;
    T* m_segmentType;

  public:
    DubinsSegmentsFunctor(const T* startPose,
                          const uint32_T numStartPoses,
                          const T* goalPose,
                          const uint32_T numGoalPoses,
                          const T turningRadius,
                          const boolean_T* allPathTypes,
                          const uint32_T numDisabledPathTypes,
                          const uint32_T numPaths,
                          const boolean_T isOptimal,
                          const uint32_T nlhs,
                          T* distance,
                          T* segmentLen,
                          T* segmentType)
        : m_startPose(startPose)
        , m_maxNumPoses(numStartPoses > numGoalPoses ? numStartPoses : numGoalPoses)
        , m_numStartPoses(numStartPoses)
        , m_numGoalPoses(numGoalPoses)
        , m_goalPose(goalPose)
        , m_turningRadius(turningRadius)
        , m_allPathTypes(allPathTypes)
        , m_numTotalPathTypes(static_cast<const uint32_T>(TotalNumPaths) - numDisabledPathTypes)
        , m_numPaths(numPaths)
        , m_isOptimal(isOptimal)
        , m_nlhs(nlhs)
        , m_distance(distance)
        , m_segmentLen(segmentLen)
        , m_segmentType(segmentType)

    {
    }

    /**
     * Functor to calculate the Dubins segment information for a
     * given range of start and goal points.
     *
     * This functor can be executed as-is (for serial execution) or
     * be part of parallel execution, e.g. through parallel_for
     *
     * @param range The range information for how many start and
     * goal locations should be processed.
     */
    void operator()(const autonomous::ParallelRange<uint32_T>& range) const {
        // The conditional statements are constant, so should be
        // compiler-optimized.
        if (m_numStartPoses >= m_numGoalPoses && m_numGoalPoses == 1) {
            // Single startPose, single goalPose
            // Multiple startPoses, single goalPose
            T a[3];
            for (uint32_T i = range.begin(); i < range.end(); ++i) {
                a[0] = m_startPose[i];
                a[1] = m_startPose[i + m_maxNumPoses];
                a[2] = m_startPose[i + 2 * m_maxNumPoses];

                std::vector<DubinsPath> path =
                    computeAllDubinsPaths(&a[0], m_goalPose, m_turningRadius, m_isOptimal,
                                          &m_allPathTypes[0], m_numTotalPathTypes);

                // Add cost, segment lengths & segment types
                addCostSegLensSegTypes(m_distance, m_segmentLen, m_segmentType, path, i, m_numPaths,
                                       m_turningRadius, m_nlhs);
            }

        } else if (m_numStartPoses < m_numGoalPoses && m_numStartPoses == 1) {
            // Single startPose, multiple goalPoses
            T b[3];
            for (uint32_T i = range.begin(); i < range.end(); ++i) {
                b[0] = m_goalPose[i];
                b[1] = m_goalPose[i + m_maxNumPoses];
                b[2] = m_goalPose[i + 2 * m_maxNumPoses];

                std::vector<DubinsPath> path =
                    computeAllDubinsPaths(m_startPose, &b[0], m_turningRadius, m_isOptimal,
                                          &m_allPathTypes[0], m_numTotalPathTypes);

                // Add cost, segment lengths & segment types
                addCostSegLensSegTypes(m_distance, m_segmentLen, m_segmentType, path, i, m_numPaths,
                                       m_turningRadius, m_nlhs);
            }

        } else {
            // Multiple startPoses, multiple goalPoses
            T a[3];
            T b[3];
            for (uint32_T i = range.begin(); i < range.end(); ++i) {
                a[0] = m_startPose[i];
                a[1] = m_startPose[i + m_maxNumPoses];
                a[2] = m_startPose[i + 2 * m_maxNumPoses];
                b[0] = m_goalPose[i];
                b[1] = m_goalPose[i + m_maxNumPoses];
                b[2] = m_goalPose[i + 2 * m_maxNumPoses];

                std::vector<DubinsPath> path =
                    computeAllDubinsPaths(&a[0], &b[0], m_turningRadius, m_isOptimal,
                                          &m_allPathTypes[0], m_numTotalPathTypes);

                // Add cost, segment lengths & segment types
                addCostSegLensSegTypes(m_distance, m_segmentLen, m_segmentType, path, i, m_numPaths,
                                       m_turningRadius, m_nlhs);
            }
        }
    }
};

/**
 * @brief Functor for calculating the distance along Dubins motion primitive segments
 *
 * This function is used to allow both parallel (with TBB) and serial execution.
 * @see autonomousDubinsDistanceCodegen_real64, autonomousDubinsDistanceCodegen_tbb_real64
 */
template <typename T>
class DubinsDistanceFunctor {
  private:
    const T* m_startPose;
    const uint32_T m_maxNumPoses;
    const uint32_T m_numStartPoses;
    const uint32_T m_numGoalPoses;
    const T* m_goalPose;
    const T m_turningRadius;
    T* m_distance;

  public:
    DubinsDistanceFunctor(const T* startPose,
                          const uint32_T numStartPoses,
                          const T* goalPose,
                          const uint32_T numGoalPoses,
                          const T turningRadius,
                          T* distance)
        : m_startPose(startPose)
        , m_maxNumPoses(numStartPoses > numGoalPoses ? numStartPoses : numGoalPoses)
        , m_numStartPoses(numStartPoses)
        , m_numGoalPoses(numGoalPoses)
        , m_goalPose(goalPose)
        , m_turningRadius(turningRadius)
        , m_distance(distance) {
    }

    /**
     * Functor to calculate the distance along Dubins segments for a
     * given range of start and goal points.
     *
     * This functor can be executed as-is (for serial execution) or
     * be part of parallel execution, e.g. through parallel_for
     *
     * @param range The range information for how many start and
     * goal locations should be processed.
     */
    void operator()(const autonomous::ParallelRange<uint32_T>& range) const {
        // The conditional statements are constant, so should be
        // compiler-optimized.

        if (m_numStartPoses >= m_numGoalPoses) {
            // Single startPose, single goalPose
            // Multiple startPoses, single goalPose
            T a[3];
            for (uint32_T i = range.begin(); i < range.end(); ++i) {
                a[0] = m_startPose[i];
                a[1] = m_startPose[i + m_maxNumPoses];
                a[2] = m_startPose[i + 2 * m_maxNumPoses];

                DubinsPath path = computeDubinsPath(&a[0], m_goalPose, m_turningRadius);
                m_distance[i] = path.length() * m_turningRadius;
            }

        } else {
            // Single startPose, multiple goalPoses
            T b[3];
            for (uint32_T i = range.begin(); i < range.end(); ++i) {
                b[0] = m_goalPose[i];
                b[1] = m_goalPose[i + m_maxNumPoses];
                b[2] = m_goalPose[i + 2 * m_maxNumPoses];

                DubinsPath path = computeDubinsPath(m_startPose, &b[0], m_turningRadius);
                m_distance[i] = path.length() * m_turningRadius;
            }
        }
    }
};

} // namespace dubins
} // namespace autonomous

#endif /* AUTONOMOUSCODEGEN_DUBINS_FUNCTORS_HPP_ */
