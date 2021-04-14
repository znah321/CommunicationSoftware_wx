/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * Functions used for Reeds-Shepp motion primitive calculations.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_REEDS_SHEPP_FUNCTORS_HPP_
#define AUTONOMOUSCODEGEN_REEDS_SHEPP_FUNCTORS_HPP_

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_constants.hpp"
#include "autonomouscodegen/autonomouscodegen_reeds_shepp.hpp"
#include "autonomouscodegen/autonomouscodegen_parallel_range.hpp"
#include "autonomouscodegen/autonomouscodegen_util.hpp"
#else
// To deal with the fact that PackNGo has no include file hierarchy during test
#include "autonomouscodegen_constants.hpp"
#include "autonomouscodegen_reeds_shepp.hpp"
#include "autonomouscodegen_parallel_range.hpp"
#include "autonomouscodegen_util.hpp"
#endif


namespace autonomous {
namespace reedsshepp {

/**
 * @brief Add extracted (path cost, path segments, path types) to output vector.
 */
template <typename T>
void addCostSegLensSegTypes(T* distances,
                            T* segmentLen,
                            T* segmentType,
                            std::vector<autonomous::reedsshepp::ReedsSheppPath> path,
                            const uint32_T m,
                            const uint32_T numPaths,
                            const T turningRadius,
                            const uint32_T numInputs) {
    // Add cost
    for (uint32_T n = 0; n < numPaths; ++n) {
        distances[n + m * numPaths] = path[n].cost() * turningRadius;
    }

    // Add segment lengths
    if (numInputs == 2) {
        for (uint32_T n = 0; n < numPaths; ++n) {
            const T* segLens = path[n].getSegmentLengths();
            for (uint32_T k = 0; k < static_cast<uint32_T>(5); ++k) {
                segmentLen[k + n * 5 + m * 5 * numPaths] = segLens[k] * turningRadius;
            }
        }
    }

    // Add segment lengths & segment type
    if (numInputs == 3) {
        for (uint32_T n = 0; n < numPaths; ++n) {
            const T* segLens = path[n].getSegmentLengths();
            for (uint32_T k = 0; k < static_cast<uint32_T>(5); ++k) {
                segmentLen[k + n * 5 + m * 5 * numPaths] = segLens[k] * turningRadius;
                segmentType[k + n * 5 + m * 5 * numPaths] = static_cast<int32_T>(
                    autonomous::reedsshepp::pathToSegment[path[n].getPathType()][k]);
            }
        }
    }
}

/**
 * @brief Functor for evaluating ReedsShepp motion primitive segments
 *
 * This function is used to allow both parallel (with TBB) and serial execution.
 * @see autonomousReedsSheppSegmentsCodegen_real64, autonomousReedsSheppSegmentsCodegen_tbb_real64
 */
template <typename T>
class ReedsSheppSegmentsFunctor {
  private:
    const T* m_startPose;
    const uint32_T m_maxNumPoses;
    const uint32_T m_numStartPoses;
    const uint32_T m_numGoalPoses;
    const T* m_goalPose;
    const T m_turningRadius;
    const T m_forwardCost;
    const T m_reverseCost;
    const boolean_T* m_allPathTypes;
    const uint32_T m_numTotalPathTypes;
    const uint32_T m_numPaths;
    const boolean_T m_isOptimal;
    const uint32_T m_nlhs;
    T* m_distance;
    T* m_segmentLen;
    T* m_segmentType;

  public:
    ReedsSheppSegmentsFunctor(const T* startPose,
                              const uint32_T numStartPoses,
                              const T* goalPose,
                              const uint32_T numGoalPoses,
                              const T turningRadius,
                              const T forwardCost,
                              const T reverseCost,
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
        , m_forwardCost(forwardCost)
        , m_reverseCost(reverseCost)
        , m_allPathTypes(allPathTypes)
        , m_numTotalPathTypes(static_cast<const uint32_T>(autonomous::reedsshepp::TotalNumPaths) -
                              numDisabledPathTypes)
        , m_numPaths(numPaths)
        , m_isOptimal(isOptimal)
        , m_nlhs(nlhs)
        , m_distance(distance)
        , m_segmentLen(segmentLen)
        , m_segmentType(segmentType)

    {
    }

    /**
     * Functor to calculate the ReedsShepp segment information for a
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

                std::vector<autonomous::reedsshepp::ReedsSheppPath> path =
                    autonomous::reedsshepp::computeAllReedsSheppPaths(
                        &a[0], m_goalPose, m_turningRadius, m_forwardCost, m_reverseCost,
                        m_isOptimal, &m_allPathTypes[0], m_numTotalPathTypes);

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

                std::vector<autonomous::reedsshepp::ReedsSheppPath> path =
                    autonomous::reedsshepp::computeAllReedsSheppPaths(
                        m_startPose, &b[0], m_turningRadius, m_forwardCost, m_reverseCost,
                        m_isOptimal, &m_allPathTypes[0], m_numTotalPathTypes);

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

                std::vector<autonomous::reedsshepp::ReedsSheppPath> path =
                    autonomous::reedsshepp::computeAllReedsSheppPaths(
                        &a[0], &b[0], m_turningRadius, m_forwardCost, m_reverseCost, m_isOptimal,
                        &m_allPathTypes[0], m_numTotalPathTypes);

                // Add cost, segment lengths & segment types
                addCostSegLensSegTypes(m_distance, m_segmentLen, m_segmentType, path, i, m_numPaths,
                                       m_turningRadius, m_nlhs);
            }
        }
    }
};


/**
 * @brief Functor for calculating the distance along ReedsShepp motion primitive segments
 *
 * This function is used to allow both parallel (with TBB) and serial execution.
 * @see autonomousReedsSheppDistanceCodegen_real64, autonomousReedsSheppDistanceCodegen_tbb_real64
 */
template <typename T>
class ReedsSheppDistanceFunctor {
  private:
    const T* m_startPose;
    const uint32_T m_maxNumPoses;
    const uint32_T m_numStartPoses;
    const uint32_T m_numGoalPoses;
    const T* m_goalPose;
    const T m_turningRadius;
    const T m_reverseCost;
    T* m_distance;

  public:
    ReedsSheppDistanceFunctor(const T* startPose,
                              const uint32_T numStartPoses,
                              const T* goalPose,
                              const uint32_T numGoalPoses,
                              const T turningRadius,
                              const T reverseCost,
                              T* distance)
        : m_startPose(startPose)
        , m_maxNumPoses(numStartPoses > numGoalPoses ? numStartPoses : numGoalPoses)
        , m_numStartPoses(numStartPoses)
        , m_numGoalPoses(numGoalPoses)
        , m_goalPose(goalPose)
        , m_turningRadius(turningRadius)
        , m_reverseCost(reverseCost)
        , m_distance(distance) {
    }

    /**
     * Functor to calculate the distance along ReedsShepp segments for a
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

                autonomous::reedsshepp::ReedsSheppPath path =
                    autonomous::reedsshepp::computeReedsSheppPath(&a[0], m_goalPose,
                                                                  m_turningRadius, m_reverseCost);
                m_distance[i] = path.cost() * m_turningRadius;
            }

        } else {
            // Single startPose, multiple goalPoses
            T b[3];
            for (uint32_T i = range.begin(); i < range.end(); ++i) {
                b[0] = m_goalPose[i];
                b[1] = m_goalPose[i + m_maxNumPoses];
                b[2] = m_goalPose[i + 2 * m_maxNumPoses];

                autonomous::reedsshepp::ReedsSheppPath path =
                    autonomous::reedsshepp::computeReedsSheppPath(m_startPose, &b[0],
                                                                  m_turningRadius, m_reverseCost);
                m_distance[i] = path.cost() * m_turningRadius;
            }
        }
    }
};

} // namespace reedsshepp
} // namespace autonomous

#endif /* AUTONOMOUSCODEGEN_REEDS_SHEPP_FUNCTORS_HPP_ */
