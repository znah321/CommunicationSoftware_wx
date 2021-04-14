/* Copyright 2017-2018 The MathWorks, Inc. */

/**
 * @file
 * Functors used for Reeds-Shepp motion primitive calculations.
 * To fully support code generation, note that this file needs to be fully
 * compliant with the C++98 standard.
 */

#ifndef AUTONOMOUSCODEGEN_REEDS_SHEPP_PRIMITIVES_H_
#define AUTONOMOUSCODEGEN_REEDS_SHEPP_PRIMITIVES_H_

#include <algorithm>
#include <vector>

#ifdef BUILDING_LIBMWAUTONOMOUSCODEGEN
#include "autonomouscodegen/autonomouscodegen_constants.hpp"             // for pi, twoPi, tooSmall
#include "autonomouscodegen/autonomouscodegen_reeds_shepp_constants.hpp" // for TotalNumPaths
#include "autonomouscodegen/autonomouscodegen_trig.hpp" // for wrapToPi, toPolar, abs
#include "autonomouscodegen/autonomouscodegen_util.hpp" // for data types
#else
// To deal with the fact that PackNGo has no include file hierarchy during test
#include "autonomouscodegen_constants.hpp"
#include "autonomouscodegen_reeds_shepp_constants.hpp"
#include "autonomouscodegen_trig.hpp"
#include "autonomouscodegen_util.hpp"
#endif

namespace autonomous {
namespace reedsshepp {

inline void tauOmega(const real64_T u,
                     const real64_T v,
                     const real64_T xi,
                     const real64_T eta,
                     const real64_T phi,
                     real64_T& tau,
                     real64_T& omega) {
    real64_T delta = autonomous::wrapToPi(u - v);
    real64_T A = sin(u) - sin(delta);
    real64_T B = cos(u) - cos(delta) - 1.0;

    real64_T t1 = atan2(eta * A - xi * B, xi * A + eta * B);
    real64_T t2 = 2.0 * (cos(delta) - cos(v) - cos(u)) + 3;

    tau = (t2 < 0) ? autonomous::wrapToPi(t1 + autonomous::pi) : autonomous::wrapToPi(t1);
    omega = autonomous::wrapToPi(tau - u + v - phi);
}

// formula 8.1 in Reeds-Shepp paper
inline boolean_T computeLpSpLpPath(const real64_T x,
                                   const real64_T y,
                                   const real64_T phi,
                                   real64_T& t,
                                   real64_T& u,
                                   real64_T& v) {
    autonomous::toPolar(x - sin(phi), y - 1.0 + cos(phi), u, t);

    if (t >= -autonomous::tooSmall) {
        v = autonomous::wrapToPi(phi - t);
        return (v >= -autonomous::tooSmall);
    }

    return false;
}

// formula 8.2
inline boolean_T computeLpSpRpPath(const real64_T x,
                                   const real64_T y,
                                   const real64_T phi,
                                   real64_T& t,
                                   real64_T& u,
                                   real64_T& v) {
    real64_T t1, u1;
    autonomous::toPolar(x + sin(phi), y - 1.0 - cos(phi), u1, t1);

    u1 = u1 * u1;

    if (u1 >= 4.0) {
        real64_T theta;

        u = sqrt(u1 - 4.);
        theta = atan2(2., u);

        t = autonomous::wrapToPi(t1 + theta);
        v = autonomous::wrapToPi(t - phi);

        return t >= -autonomous::tooSmall && v >= -autonomous::tooSmall;
    }
    return false;
}

// formula 8.3 / 8.4  *** TYPO IN PAPER ***
inline boolean_T computeLpRmLPath(const real64_T x,
                                  const real64_T y,
                                  const real64_T phi,
                                  real64_T& t,
                                  real64_T& u,
                                  real64_T& v) {
    real64_T xi = x - sin(phi);
    real64_T eta = y - 1.0 + cos(phi);

    real64_T u1, theta;
    autonomous::toPolar(xi, eta, u1, theta);

    if (u1 <= 4.0) {
        u = -2.0 * asin(0.25 * u1);

        t = autonomous::wrapToPi(theta + 0.5 * u + autonomous::pi);
        v = autonomous::wrapToPi(phi - t + u);

        return t >= -autonomous::tooSmall && u <= autonomous::tooSmall;
    }
    return false;
}

// formula 8.7
inline boolean_T computeLpRupLumRmPath(const real64_T x,
                                       const real64_T y,
                                       const real64_T phi,
                                       real64_T& t,
                                       real64_T& u,
                                       real64_T& v) {
    real64_T xi = x + sin(phi);
    real64_T eta = y - 1.0 - cos(phi);
    real64_T rho = 0.25 * (2.0 + sqrt(xi * xi + eta * eta));

    if (rho <= 1.) {
        u = acos(rho);

        tauOmega(u, -u, xi, eta, phi, t, v);

        return t >= -autonomous::tooSmall && v <= autonomous::tooSmall;
    }
    return false;
}

// formula 8.8
inline boolean_T computeLpRumLumRpPath(const real64_T x,
                                       const real64_T y,
                                       const real64_T phi,
                                       real64_T& t,
                                       real64_T& u,
                                       real64_T& v) {
    real64_T xi = x + sin(phi);
    real64_T eta = y - 1.0 - cos(phi);
    real64_T rho = (20.0 - xi * xi - eta * eta) / 16.0;

    if (rho >= 0 && rho <= 1) {
        u = -acos(rho);

        if (u >= -.5 * autonomous::pi) {
            tauOmega(u, u, xi, eta, phi, t, v);

            return t >= -autonomous::tooSmall && v >= -autonomous::tooSmall;
        }
    }
    return false;
}

// formula 8.9
inline boolean_T computeLpRmSmLmPath(const real64_T x,
                                     const real64_T y,
                                     const real64_T phi,
                                     real64_T& t,
                                     real64_T& u,
                                     real64_T& v) {
    real64_T xi = x - sin(phi);
    real64_T eta = y - 1.0 + cos(phi);

    real64_T rho, theta;
    autonomous::toPolar(xi, eta, rho, theta);

    if (rho >= 2.0) {
        real64_T r = sqrt(rho * rho - 4.0);
        u = 2.0 - r;

        t = autonomous::wrapToPi(theta + atan2(r, -2.0));
        v = autonomous::wrapToPi(phi - 0.5 * autonomous::pi - t);

        return t >= -autonomous::tooSmall && u <= autonomous::tooSmall && v <= autonomous::tooSmall;
    }
    return false;
}

// formula 8.10
inline boolean_T computeLpRmSmRmPath(const real64_T x,
                                     const real64_T y,
                                     const real64_T phi,
                                     real64_T& t,
                                     real64_T& u,
                                     real64_T& v) {
    real64_T xi = x + sin(phi);
    real64_T eta = y - 1.0 - cos(phi);

    real64_T rho, theta;
    autonomous::toPolar(-eta, xi, rho, theta);

    if (rho >= 2.0) {
        t = theta;

        u = 2.0 - rho;
        v = autonomous::wrapToPi(t + 0.5 * autonomous::pi - phi);

        return t >= -autonomous::tooSmall && u <= autonomous::tooSmall && v <= autonomous::tooSmall;
    }
    return false;
}

// formula 8.11 *** TYPO IN PAPER ***
inline boolean_T computeLpRmSLmRpPath(const real64_T x,
                                      const real64_T y,
                                      const real64_T phi,
                                      real64_T& t,
                                      real64_T& u,
                                      real64_T& v) {
    real64_T xi = x + sin(phi);
    real64_T eta = y - 1.0 - cos(phi);

    real64_T rho, theta;
    autonomous::toPolar(xi, eta, rho, theta);

    if (rho >= 2.0) {
        u = 4.0 - sqrt(rho * rho - 4.0);

        if (u <= autonomous::tooSmall) {
            t = autonomous::wrapToPi(atan2((4 - u) * xi - 2 * eta, -2 * xi + (u - 4) * eta));
            v = autonomous::wrapToPi(t - phi);

            return t >= -autonomous::tooSmall && v >= -autonomous::tooSmall;
        }
    }
    return false;
}

/*
 * RSPathType - Enumeration for holding path type.
 */
enum RSPathType {
    LRLNN = 0,
    RLRNN,
    LRLRN,
    RLRLN,
    LRSLN,
    RLSRN,
    LSRLN,
    RSLRN,
    LRSRN,
    RLSLN,
    RSRLN,
    LSLRN,
    LSRNN,
    RSLNN,
    LSLNN,
    RSRNN,
    LRSLR,
    RLSRL
};

/*
 * ReedsSheppPath - Encapsulate normalized Reeds-Shepp path.
 */
class ReedsSheppPath {

  public:
    ReedsSheppPath(const real64_T firstSegment = inf,
                   const real64_T secondSegment = inf,
                   const real64_T thirdSegment = inf,
                   const real64_T fourthSegment = inf,
                   const real64_T fifthSegment = inf,
                   const RSPathType pathType = autonomous::reedsshepp::LRLNN,
                   const real64_T forwardCost = 1.0,
                   const real64_T reverseCost = 1.0)
        : pathType_(pathType)
        , reverseCost_(reverseCost)
        , forwardCost_(forwardCost) {

        segmentLengths_[0] = firstSegment;
        segmentLengths_[1] = secondSegment;
        segmentLengths_[2] = thirdSegment;
        segmentLengths_[3] = fourthSegment;
        segmentLengths_[4] = fifthSegment;
    }

    real64_T length() const {
        return autonomous::abs(segmentLengths_[0]) + autonomous::abs(segmentLengths_[1]) +
               autonomous::abs(segmentLengths_[2]) + autonomous::abs(segmentLengths_[3]) +
               autonomous::abs(segmentLengths_[4]);
    }

    real64_T cost() const {
        real64_T len = 0.0;
        for (uint32_T i = 0; i < 5; ++i) {
            if (std::signbit(segmentLengths_[i])) {
                len += autonomous::abs(reverseCost_ * segmentLengths_[i]);
            } else {
                len += autonomous::abs(forwardCost_ * segmentLengths_[i]);
            }
        }
        return len;
    }

    RSPathType getPathType() const {
        return pathType_;
    }

    const real64_T* getSegmentLengths() const {

        return &segmentLengths_[0];
    }

  private:
    RSPathType pathType_;
    real64_T segmentLengths_[5];
    real64_T reverseCost_;
    real64_T forwardCost_;
};

// Compute for shortest path types
/*
 * computeCSCPath - compute ReedsSheppPath for CSCPath.
 */
inline void computeCSCPath(const real64_T x,
                           const real64_T y,
                           const real64_T phi,
                           const real64_T reverseCost,
                           ReedsSheppPath& path) {
    real64_T t, u, v, L;
    real64_T Lmin = path.length();

    if (computeLpSpLpPath(x, y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::LSLNN, 1, reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpSpLpPath(-x, y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::LSLNN, 1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpSpLpPath(x, -y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::RSRNN, 1, reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpSpLpPath(-x, -y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::RSRNN, 1, reverseCost);
        Lmin = L;
    }

    if (computeLpSpRpPath(x, y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::LSRNN, 1, reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpSpRpPath(-x, y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::LSRNN, 1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpSpRpPath(x, -y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::RSLNN, 1, reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpSpRpPath(-x, -y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::RSLNN, 1, reverseCost);
    }
}

/*
 * computeCCCPath - compute ReedsSheppPath for CCCPath.
 */
inline void computeCCCPath(const real64_T x,
                           const real64_T y,
                           const real64_T phi,
                           const real64_T reverseCost,
                           ReedsSheppPath& path) {
    real64_T t, u, v, L;
    real64_T Lmin = path.length();

    if (computeLpRmLPath(x, y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::LRLNN, 1, reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRmLPath(-x, y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::LRLNN, 1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRmLPath(x, -y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::RLRNN, 1, reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRmLPath(-x, -y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::RLRNN, 1, reverseCost);
        Lmin = L;
    }

    // backwards
    real64_T xb = x * cos(phi) + y * sin(phi);
    real64_T yb = x * sin(phi) - y * cos(phi);

    if (computeLpRmLPath(xb, yb, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(v, u, t, 0.0, 0.0, autonomous::reedsshepp::LRLNN, 1, reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRmLPath(-xb, yb, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-v, -u, -t, 0.0, 0.0, autonomous::reedsshepp::LRLNN, 1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRmLPath(xb, -yb, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(v, u, t, 0.0, 0.0, autonomous::reedsshepp::RLRNN, 1, reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRmLPath(-xb, -yb, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-v, -u, -t, 0.0, 0.0, autonomous::reedsshepp::RLRNN, 1, reverseCost);
    }
}

/*
 * computeCCCCPath - compute ReedsSheppPath for CCCCPath.
 */
inline void computeCCCCPath(const real64_T x,
                            const real64_T y,
                            const real64_T phi,
                            const real64_T reverseCost,
                            ReedsSheppPath& path) {
    real64_T t, u, v, L;
    real64_T Lmin = path.length();

    if (computeLpRupLumRmPath(x, y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + 2.0 * autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, -u, v, 0.0, autonomous::reedsshepp::LRLRN, 1, reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRupLumRmPath(-x, y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + 2.0 * autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, u, -v, 0.0, autonomous::reedsshepp::LRLRN, 1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRupLumRmPath(x, -y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + 2.0 * autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, -u, v, 0.0, autonomous::reedsshepp::RLRLN, 1, reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRupLumRmPath(-x, -y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + 2.0 * autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, u, -v, 0.0, autonomous::reedsshepp::RLRLN, 1, reverseCost);
        Lmin = L;
    }

    if (computeLpRumLumRpPath(x, y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + 2.0 * autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, u, v, 0.0, autonomous::reedsshepp::LRLRN, 1, reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRumLumRpPath(-x, y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + 2.0 * autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, -u, -v, 0.0, autonomous::reedsshepp::LRLRN, 1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRumLumRpPath(x, -y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + 2.0 * autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, u, u, v, 0.0, autonomous::reedsshepp::RLRLN, 1, reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRumLumRpPath(-x, -y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + 2.0 * autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, -u, -u, -v, 0.0, autonomous::reedsshepp::RLRLN, 1, reverseCost);
    }
}

/*
 * computeCCSCPath - compute ReedsSheppPath for CCSCPath.
 */
inline void computeCCSCPath(const real64_T x,
                            const real64_T y,
                            const real64_T phi,
                            const real64_T reverseCost,
                            ReedsSheppPath& path) {
    real64_T t, u, v, L;
    real64_T Lmin = path.length() - 0.5 * autonomous::pi;

    if (computeLpRmSmLmPath(x, y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, -0.5 * autonomous::pi, u, v, 0.0, autonomous::reedsshepp::LRSLN, 1,
                              reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRmSmLmPath(-x, y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, -v, 0.0, autonomous::reedsshepp::LRSLN,
                              1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRmSmLmPath(x, -y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, -0.5 * autonomous::pi, u, v, 0.0, autonomous::reedsshepp::RLSRN, 1,
                              reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRmSmLmPath(-x, -y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, -v, 0.0, autonomous::reedsshepp::RLSRN,
                              1, reverseCost);
        Lmin = L;
    }

    if (computeLpRmSmRmPath(x, y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, -0.5 * autonomous::pi, u, v, 0.0, autonomous::reedsshepp::LRSRN, 1,
                              reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRmSmRmPath(-x, y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, -v, 0.0, autonomous::reedsshepp::LRSRN,
                              1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRmSmRmPath(x, -y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, -0.5 * autonomous::pi, u, v, 0.0, autonomous::reedsshepp::RLSLN, 1,
                              reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRmSmRmPath(-x, -y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, -v, 0.0, autonomous::reedsshepp::RLSLN,
                              1, reverseCost);
        Lmin = L;
    }

    // backwards
    real64_T xb = x * cos(phi) + y * sin(phi);
    real64_T yb = x * sin(phi) - y * cos(phi);

    if (computeLpRmSmLmPath(xb, yb, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(v, u, -0.5 * autonomous::pi, t, 0.0, autonomous::reedsshepp::LSRLN, 1,
                              reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRmSmLmPath(-xb, yb, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-v, -u, 0.5 * autonomous::pi, -t, 0.0, autonomous::reedsshepp::LSRLN,
                              1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRmSmLmPath(xb, -yb, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(v, u, -0.5 * autonomous::pi, t, 0.0, autonomous::reedsshepp::RSLRN, 1,
                              reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRmSmLmPath(-xb, -yb, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-v, -u, 0.5 * autonomous::pi, -t, 0.0, autonomous::reedsshepp::RSLRN,
                              1, reverseCost);
        Lmin = L;
    }

    if (computeLpRmSmRmPath(xb, yb, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(v, u, -0.5 * autonomous::pi, t, 0.0, autonomous::reedsshepp::RSRLN, 1,
                              reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRmSmRmPath(-xb, yb, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-v, -u, 0.5 * autonomous::pi, -t, 0.0, autonomous::reedsshepp::RSRLN,
                              1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRmSmRmPath(xb, -yb, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(v, u, -0.5 * autonomous::pi, t, 0.0, autonomous::reedsshepp::LSLRN, 1,
                              reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRmSmRmPath(-xb, -yb, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-v, -u, 0.5 * autonomous::pi, -t, 0.0, autonomous::reedsshepp::LSLRN,
                              1, reverseCost);
    }
}

/*
 * computeCCSCCPath - compute ReedsSheppPath for CCSCCPath.
 */
inline void computeCCSCCPath(const real64_T x,
                             const real64_T y,
                             const real64_T phi,
                             const real64_T reverseCost,
                             ReedsSheppPath& path) {
    real64_T t, u, v, L;
    real64_T Lmin = path.length() - autonomous::pi;

    if (computeLpRmSLmRpPath(x, y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, -0.5 * autonomous::pi, u, -0.5 * autonomous::pi, v,
                              autonomous::reedsshepp::LRSLR, 1, reverseCost);
        Lmin = L;
    }

    // time flip
    if (computeLpRmSLmRpPath(-x, y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, 0.5 * autonomous::pi, -v,
                              autonomous::reedsshepp::LRSLR, 1, reverseCost);
        Lmin = L;
    }

    // reflect
    if (computeLpRmSLmRpPath(x, -y, -phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(t, -0.5 * autonomous::pi, u, -0.5 * autonomous::pi, v,
                              autonomous::reedsshepp::RLSRL, 1, reverseCost);
        Lmin = L;
    }

    // time flip + reflect
    if (computeLpRmSLmRpPath(-x, -y, phi, t, u, v) &&
        Lmin > (L = autonomous::abs(t) + autonomous::abs(u) + autonomous::abs(v))) {
        path = ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, 0.5 * autonomous::pi, -v,
                              autonomous::reedsshepp::RLSRL, 1, reverseCost);
    }
}

/*
 * Compute all path types.
 */

/*
 * computeCSCPath - compute ReedsSheppPath for CSCPath.
 */
inline void computeCSCPathAll(const real64_T x,
                              const real64_T y,
                              const real64_T phi,
                              const real64_T forwardCost,
                              const real64_T reverseCost,
                              const boolean_T allPathTypes[TotalNumPaths],
                              std::vector<ReedsSheppPath>* path) {
    real64_T t, u, v;

    if (allPathTypes[0]) {
        computeLpSpLpPath(x, y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::LSLNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, 0.0, 0.0, autonomous::reedsshepp::LSLNN,
                                             forwardCost, reverseCost));
    }

    // time flip
    if (allPathTypes[1]) {
        computeLpSpLpPath(-x, y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::LSLNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, 0.0, 0.0,
                                             autonomous::reedsshepp::LSLNN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[2]) {
        computeLpSpLpPath(x, -y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::RSRNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, 0.0, 0.0, autonomous::reedsshepp::RSRNN,
                                             forwardCost, reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[3]) {
        computeLpSpLpPath(-x, -y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::RSRNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, 0.0, 0.0,
                                             autonomous::reedsshepp::RSRNN, forwardCost,
                                             reverseCost));
    }

    if (allPathTypes[4]) {
        computeLpSpRpPath(x, y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::LSRNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, 0.0, 0.0, autonomous::reedsshepp::LSRNN,
                                             forwardCost, reverseCost));
    }

    // time flip
    if (allPathTypes[5]) {
        computeLpSpRpPath(-x, y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::LSRNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, 0.0, 0.0,
                                             autonomous::reedsshepp::LSRNN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[6]) {
        computeLpSpRpPath(x, -y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::RSLNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, 0.0, 0.0, autonomous::reedsshepp::RSLNN,
                                             forwardCost, reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[7]) {
        computeLpSpRpPath(-x, -y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::RSLNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, 0.0, 0.0,
                                             autonomous::reedsshepp::RSLNN, forwardCost,
                                             reverseCost));
    }
}

/*
 * computeCCCPath - compute ReedsSheppPath for CCCPath.
 */
inline void computeCCCPathAll(const real64_T x,
                              const real64_T y,
                              const real64_T phi,
                              const real64_T forwardCost,
                              const real64_T reverseCost,
                              const boolean_T allPathTypes[TotalNumPaths],
                              std::vector<ReedsSheppPath>* path) {
    real64_T t, u, v;

    if (allPathTypes[8]) {
        computeLpRmLPath(x, y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::LRLNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, 0.0, 0.0, autonomous::reedsshepp::LRLNN,
                                             forwardCost, reverseCost));
    }

    // time flip
    if (allPathTypes[9]) {
        computeLpRmLPath(-x, y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::LRLNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, 0.0, 0.0,
                                             autonomous::reedsshepp::LRLNN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[10]) {
        computeLpRmLPath(x, -y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, v, 0.0, 0.0, autonomous::reedsshepp::RLRNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, 0.0, 0.0, autonomous::reedsshepp::RLRNN,
                                             forwardCost, reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[11]) {
        computeLpRmLPath(-x, -y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, -v, 0.0, 0.0, autonomous::reedsshepp::RLRNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, 0.0, 0.0,
                                             autonomous::reedsshepp::RLRNN, forwardCost,
                                             reverseCost));
    }

    // backwards
    real64_T xb = x * cos(phi) + y * sin(phi);
    real64_T yb = x * sin(phi) - y * cos(phi);

    if (allPathTypes[12]) {
        computeLpRmLPath(xb, yb, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(v, u, t, 0.0, 0.0, autonomous::reedsshepp::LRLNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, 0.0, 0.0, autonomous::reedsshepp::LRLNN,
                                             forwardCost, reverseCost));
    }

    // time flip
    if (allPathTypes[13]) {
        computeLpRmLPath(-xb, yb, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-v, -u, -t, 0.0, 0.0, autonomous::reedsshepp::LRLNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, 0.0, 0.0,
                                             autonomous::reedsshepp::LRLNN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[14]) {
        computeLpRmLPath(xb, -yb, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(v, u, t, 0.0, 0.0, autonomous::reedsshepp::RLRNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, 0.0, 0.0, autonomous::reedsshepp::RLRNN,
                                             forwardCost, reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[15]) {
        computeLpRmLPath(-xb, -yb, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-v, -u, -t, 0.0, 0.0, autonomous::reedsshepp::RLRNN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, 0.0, 0.0,
                                             autonomous::reedsshepp::RLRNN, forwardCost,
                                             reverseCost));
    }
}

/*
 * computeCCCCPath - compute ReedsSheppPath for CCCCPath.
 */
inline void computeCCCCPathAll(const real64_T x,
                               const real64_T y,
                               const real64_T phi,
                               const real64_T forwardCost,
                               const real64_T reverseCost,
                               const boolean_T allPathTypes[TotalNumPaths],
                               std::vector<ReedsSheppPath>* path) {
    real64_T t, u, v;

    if (allPathTypes[16]) {
        computeLpRupLumRmPath(x, y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, -u, v, 0.0, autonomous::reedsshepp::LRLRN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, -inf, inf, 0.0,
                                             autonomous::reedsshepp::LRLRN, forwardCost,
                                             reverseCost));
    }

    // time flip
    if (allPathTypes[17]) {
        computeLpRupLumRmPath(-x, y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, u, -v, 0.0, autonomous::reedsshepp::LRLRN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, inf, -inf, 0.0,
                                             autonomous::reedsshepp::LRLRN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[18]) {
        computeLpRupLumRmPath(x, -y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, -u, v, 0.0, autonomous::reedsshepp::RLRLN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, -inf, inf, 0.0,
                                             autonomous::reedsshepp::RLRLN, forwardCost,
                                             reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[19]) {
        computeLpRupLumRmPath(-x, -y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, u, -v, 0.0, autonomous::reedsshepp::RLRLN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, inf, -inf, 0.0,
                                             autonomous::reedsshepp::RLRLN, forwardCost,
                                             reverseCost));
    }

    if (allPathTypes[20]) {
        computeLpRumLumRpPath(x, y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, u, v, 0.0, autonomous::reedsshepp::LRLRN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, inf, 0.0, autonomous::reedsshepp::LRLRN,
                                             forwardCost, reverseCost));
    }

    // time flip
    if (allPathTypes[21]) {
        computeLpRumLumRpPath(-x, y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, -u, -v, 0.0, autonomous::reedsshepp::LRLRN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, -inf, 0.0,
                                             autonomous::reedsshepp::LRLRN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[22]) {
        computeLpRumLumRpPath(x, -y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, u, u, v, 0.0, autonomous::reedsshepp::RLRLN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, inf, inf, 0.0, autonomous::reedsshepp::RLRLN,
                                             forwardCost, reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[23]) {
        computeLpRumLumRpPath(-x, -y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, -u, -u, -v, 0.0, autonomous::reedsshepp::RLRLN,
                                             forwardCost, reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, -inf, -inf, 0.0,
                                             autonomous::reedsshepp::RLRLN, forwardCost,
                                             reverseCost));
    }
}

/*
 * computeCCSCPath - compute ReedsSheppPath for CCSCPath.
 */
inline void computeCCSCPathAll(const real64_T x,
                               const real64_T y,
                               const real64_T phi,
                               const real64_T forwardCost,
                               const real64_T reverseCost,
                               const boolean_T allPathTypes[TotalNumPaths],
                               std::vector<ReedsSheppPath>* path) {
    real64_T t, u, v;

    if (allPathTypes[24]) {
        computeLpRmSmLmPath(x, y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, -0.5 * autonomous::pi, u, v, 0.0,
                                             autonomous::reedsshepp::LRSLN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, -0.5 * autonomous::pi, inf, inf, 0.0,
                                             autonomous::reedsshepp::LRSLN, forwardCost,
                                             reverseCost));
    }

    // time flip
    if (allPathTypes[25]) {
        computeLpRmSmLmPath(-x, y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, -v, 0.0,
                                             autonomous::reedsshepp::LRSLN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, 0.5 * autonomous::pi, -inf, -inf, 0.0,
                                             autonomous::reedsshepp::LRSLN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[26]) {
        computeLpRmSmLmPath(x, -y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, -0.5 * autonomous::pi, u, v, 0.0,
                                             autonomous::reedsshepp::RLSRN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, -0.5 * autonomous::pi, inf, inf, 0.0,
                                             autonomous::reedsshepp::RLSRN, forwardCost,
                                             reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[27]) {
        computeLpRmSmLmPath(-x, -y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, -v, 0.0,
                                             autonomous::reedsshepp::RLSRN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, 0.5 * autonomous::pi, -inf, -inf, 0.0,
                                             autonomous::reedsshepp::RLSRN, forwardCost,
                                             reverseCost));
    }

    if (allPathTypes[28]) {
        computeLpRmSmRmPath(x, y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, -0.5 * autonomous::pi, u, v, 0.0,
                                             autonomous::reedsshepp::LRSRN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, -0.5 * autonomous::pi, inf, inf, 0.0,
                                             autonomous::reedsshepp::LRSRN, forwardCost,
                                             reverseCost));
    }

    // time flip
    if (allPathTypes[29]) {
        computeLpRmSmRmPath(-x, y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, -v, 0.0,
                                             autonomous::reedsshepp::LRSRN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, 0.5 * autonomous::pi, -inf, -inf, 0.0,
                                             autonomous::reedsshepp::LRSRN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[30]) {
        computeLpRmSmRmPath(x, -y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, -0.5 * autonomous::pi, u, v, 0.0,
                                             autonomous::reedsshepp::RLSLN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, -0.5 * autonomous::pi, inf, inf, 0.0,
                                             autonomous::reedsshepp::RLSLN, forwardCost,
                                             reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[31]) {
        computeLpRmSmRmPath(-x, -y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, -v, 0.0,
                                             autonomous::reedsshepp::RLSLN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, 0.5 * autonomous::pi, -inf, -inf, 0.0,
                                             autonomous::reedsshepp::RLSLN, forwardCost,
                                             reverseCost));
    }

    // backwards
    real64_T xb = x * cos(phi) + y * sin(phi);
    real64_T yb = x * sin(phi) - y * cos(phi);

    if (allPathTypes[32]) {
        computeLpRmSmLmPath(xb, yb, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(v, u, -0.5 * autonomous::pi, t, 0.0,
                                             autonomous::reedsshepp::LSRLN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, -0.5 * autonomous::pi, inf, 0.0,
                                             autonomous::reedsshepp::LSRLN, forwardCost,
                                             reverseCost));
    }

    // time flip
    if (allPathTypes[33]) {
        computeLpRmSmLmPath(-xb, yb, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-v, -u, 0.5 * autonomous::pi, -t, 0.0,
                                             autonomous::reedsshepp::LSRLN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, 0.5 * autonomous::pi, -inf, 0.0,
                                             autonomous::reedsshepp::LSRLN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[34]) {
        computeLpRmSmLmPath(xb, -yb, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(v, u, -0.5 * autonomous::pi, t, 0.0,
                                             autonomous::reedsshepp::RSLRN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, -0.5 * autonomous::pi, inf, 0.0,
                                             autonomous::reedsshepp::RSLRN, forwardCost,
                                             reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[35]) {
        computeLpRmSmLmPath(-xb, -yb, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-v, -u, 0.5 * autonomous::pi, -t, 0.0,
                                             autonomous::reedsshepp::RSLRN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, 0.5 * autonomous::pi, -inf, 0.0,
                                             autonomous::reedsshepp::RSLRN, forwardCost,
                                             reverseCost));
    }

    if (allPathTypes[36]) {
        computeLpRmSmRmPath(xb, yb, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(v, u, -0.5 * autonomous::pi, t, 0.0,
                                             autonomous::reedsshepp::RSRLN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, -0.5 * autonomous::pi, inf, 0.0,
                                             autonomous::reedsshepp::RSRLN, forwardCost,
                                             reverseCost));
    }

    // time flip
    if (allPathTypes[37]) {
        computeLpRmSmRmPath(-xb, yb, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-v, -u, 0.5 * autonomous::pi, -t, 0.0,
                                             autonomous::reedsshepp::RSRLN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, 0.5 * autonomous::pi, -inf, 0.0,
                                             autonomous::reedsshepp::RSRLN, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[38]) {
        computeLpRmSmRmPath(xb, -yb, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(v, u, -0.5 * autonomous::pi, t, 0.0,
                                             autonomous::reedsshepp::LSLRN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, inf, -0.5 * autonomous::pi, inf, 0.0,
                                             autonomous::reedsshepp::LSLRN, forwardCost,
                                             reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[39]) {
        computeLpRmSmRmPath(-xb, -yb, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-v, -u, 0.5 * autonomous::pi, -t, 0.0,
                                             autonomous::reedsshepp::LSLRN, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, -inf, 0.5 * autonomous::pi, -inf, 0.0,
                                             autonomous::reedsshepp::LSLRN, forwardCost,
                                             reverseCost));
    }
}

/*
 * computeCCSCCPath - compute ReedsSheppPath for CCSCCPath.
 */
inline void computeCCSCCPathAll(const real64_T x,
                                const real64_T y,
                                const real64_T phi,
                                real64_T const forwardCost,
                                const real64_T reverseCost,
                                const boolean_T allPathTypes[TotalNumPaths],
                                std::vector<ReedsSheppPath>* path) {
    real64_T t, u, v;

    if (allPathTypes[40]) {
        computeLpRmSLmRpPath(x, y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, -0.5 * autonomous::pi, u, -0.5 * autonomous::pi, v,
                                             autonomous::reedsshepp::LRSLR, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, -0.5 * autonomous::pi, inf, -0.5 * autonomous::pi,
                                             inf, autonomous::reedsshepp::LRSLR, forwardCost,
                                             reverseCost));
    }

    // time flip
    if (allPathTypes[41]) {
        computeLpRmSLmRpPath(-x, y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, 0.5 * autonomous::pi, -v,
                                             autonomous::reedsshepp::LRSLR, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, 0.5 * autonomous::pi, -inf, 0.5 * autonomous::pi,
                                             -inf, autonomous::reedsshepp::LRSLR, forwardCost,
                                             reverseCost));
    }

    // reflect
    if (allPathTypes[42]) {
        computeLpRmSLmRpPath(x, -y, -phi, t, u, v)
            ? path->push_back(ReedsSheppPath(t, -0.5 * autonomous::pi, u, -0.5 * autonomous::pi, v,
                                             autonomous::reedsshepp::RLSRL, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(inf, -0.5 * autonomous::pi, inf, -0.5 * autonomous::pi,
                                             inf, autonomous::reedsshepp::RLSRL, forwardCost,
                                             reverseCost));
    }

    // time flip + reflect
    if (allPathTypes[43]) {
        computeLpRmSLmRpPath(-x, -y, phi, t, u, v)
            ? path->push_back(ReedsSheppPath(-t, 0.5 * autonomous::pi, -u, 0.5 * autonomous::pi, -v,
                                             autonomous::reedsshepp::RLSRL, forwardCost,
                                             reverseCost))
            : path->push_back(ReedsSheppPath(-inf, 0.5 * autonomous::pi, -inf, 0.5 * autonomous::pi,
                                             -inf, autonomous::reedsshepp::RLSRL, forwardCost,
                                             reverseCost));
    }
}
} // namespace reedsshepp
} // namespace autonomous

#endif /* AUTONOMOUSCODEGEN_REEDS_SHEPP_PRIMITIVES_H_ */
