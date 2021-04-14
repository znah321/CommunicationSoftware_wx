/* Copyright 2018 The MathWorks, Inc. */

/**
 * @file
 * Open-interval range that can be used for parallel or serial execution.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_PARALLEL_RANGE_H_
#define AUTONOMOUSCODEGEN_PARALLEL_RANGE_H_

// TBB defines a dummy datatype (tbb::split) to differentiate
// splitting constructors from all the rest.  Splittability is
// potentially useful for contexts outside of TBB, so define a dummy
// class if TBB isn't available.
#if defined(TBB_AVAILABLE)
#include "tbb/tbb_stddef.h"
#else
namespace tbb {
typedef struct split {
} split;
} // namespace tbb
#endif

namespace autonomous {

/**
 * An open-interval range [begin, end) that can be used for parallel or
 * serial execution.
 *
 * This implementation follows the "Range Concept" requirements spelled out by
 * TBB (https://software.intel.com/en-us/node/506143#range_concept), but also
 * compiles and runs if no TBB is available.
 */
template <typename T>
class ParallelRange {
  public:
    typedef T const_iterator;

    typedef uint32_T uint32_Type;

    ParallelRange()
        : m_end(static_cast<T>(0))
        , m_begin(static_cast<T>(0))
        , m_grainSize(1) {
    }

    /**
     * Construct range over the half-open interval [begin,end), with the given
     * grain size.
     *
     * @param begin Start of range (inclusive)
     * @param end End of range (exclusive, open interval)
     * @param grainSize Allow subdivision of the full range based on the grain size.
     */
    ParallelRange(T begin, T end, uint32_Type grainSize = 1)
        : m_end(end)
        , m_begin(begin)
        , m_grainSize(grainSize) {
    }

    /**
     * Inclusive lower bound on range.
     */
    const_iterator begin() const {
        return m_begin;
    }

    /**
     * Exclusive upper bound on range.
     */
    const_iterator end() const {
        return m_end;
    }

    uint32_Type size() const {
        return uint32_Type(m_end - m_begin);
    }

    uint32_Type grainsize() const {
        return m_grainSize;
    }

    //------------------------------------------------------------------------
    // Methods that implement Range concept
    //------------------------------------------------------------------------

    /// True if range is empty.
    boolean_T empty() const {
        return !(m_begin < m_end);
    }

    /// True if range is divisible.
    boolean_T is_divisible() const {
        return m_grainSize < size();
    }

    /// Split range.
    ParallelRange(ParallelRange& r, tbb::split)
        : m_end(r.m_end)
        , m_begin(splitRange(r))
        , m_grainSize(r.m_grainSize) {
    }

  private:
    T m_end;                 ///< End of range (exclusive)
    T m_begin;               ///< Start of range (inclusive)
    uint32_Type m_grainSize; ///< Grain size used for subdivision

    /// Auxiliary function used by forking constructor.
    static T splitRange(ParallelRange& r) {
        T middle = r.m_begin + (r.m_end - r.m_begin) / 2u;
        r.m_end = middle;
        return middle;
    }
};
} // namespace autonomous

#endif
