/*
 * matrixOperations.hpp
 *
 * Copyright 2019 The MathWorks, Inc.
 */

#ifndef matrixOperations_hpp
#define matrixOperations_hpp

#include <cstddef>
#include "cppshrhelp.hpp"

class DLL_EXPORT Mat {
    int * arr;
    size_t len;

public:
    // initialize arr
    void setMat(int src[], size_t len);

    // get arr pointer
    const int * const getMat(size_t len) const;

    // get arr length
    size_t getLength() const;

    // copy arr to dest
    void copyMat(int * dest, size_t len);
};

// add contents of arr
DLL_EXPORT int addMat(const Mat * mat);

// update contents of arr by X
DLL_EXPORT void updateMatByX(Mat& mat, int X);

// expand arr
DLL_EXPORT void updateMatBySize(Mat& mat, int * arr, size_t len);

#endif

