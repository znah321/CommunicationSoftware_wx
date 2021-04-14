/*
 * matrixOperations.cpp
 *
 * Copyright 2019 The MathWorks, Inc.
 */

#define EXPORT
#include "matrixOperations.hpp"

void Mat::setMat(int src[], size_t len)
{
    if (this->arr != src && len != 0)
    {
        if (this->len != 0)
        {
            delete[] this->arr;
            this->len = 0;
        }
        this->arr = new int[len];
        for (size_t idx = 0; idx < len; idx++)
            this->arr[idx] = src[idx];
        this->len = len;
    }
}

const int * const Mat::getMat(size_t len) const
{
    return this->arr;
}

size_t Mat::getLength() const
{
    return this->len;
}

void Mat::copyMat(int * dest, size_t len)
{
    if (len != 0)
    {
        if (dest == NULL)
            dest = new int[len];
        for (size_t idx = 0; idx < len; idx++)
            dest[idx] = this->arr[idx];
    }
}

DLL_EXPORT int addMat(const Mat * mat)
{
    int res = 0;
    if (mat == NULL) return res;
    const int * const matArr = mat->getMat(mat->getLength());
    for (size_t idx = 0; idx < mat->getLength(); idx++)
        res += matArr[idx];
    return res;
}

DLL_EXPORT void updateMatByX(Mat& mat, int X)
{
    int * newArr = new int[mat.getLength()];
    mat.copyMat(newArr, mat.getLength());
    for (size_t idx = 0; idx < mat.getLength(); idx++)
        newArr[idx] += X;
    mat.setMat(newArr, mat.getLength());
}

DLL_EXPORT void updateMatBySize(Mat& mat, int * arr, size_t len)
{
    size_t newLen = mat.getLength() + len;
    int * newArr = new int[newLen];
    mat.copyMat(newArr, mat.getLength());
    for (size_t idx = 0; idx < len; idx++)
        newArr[mat.getLength() + idx] = arr[idx];
    mat.setMat(newArr, newLen);
}


