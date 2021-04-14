/*
 * Read a structure array STRUCT from MATFILE and
 * sum the first real numeric value in FIELD for each 
 * record/element in the array.
 *
 * Calling syntax:
 *
 *   matreadstructarray <matfile> <struct> <field>
 *
 * Copyright 2012 The MathWorks, Inc.
 */

#include <stdlib.h>
#include "mat.h"

/* Analyze field FNAME in struct array SPTR. */
static void
analyzestructarray(const mxArray *sPtr, const char *fName)
{
    mwSize nElements;       /* number of elements in array */
    mwIndex eIdx;           /* element index */
    const mxArray *fPtr;    /* field pointer */
    double *realPtr;        /* pointer to data */
    double total;           /* value to calculate */
    
    total = 0;
    nElements = (mwSize)mxGetNumberOfElements(sPtr);
    for (eIdx = 0; eIdx < nElements; eIdx++) {
        fPtr = mxGetField(sPtr, eIdx, fName);
        if ((fPtr != NULL) 
            && (mxGetClassID(fPtr) == mxDOUBLE_CLASS) 
            && (!mxIsComplex(fPtr))) 
        {
            realPtr = mxGetPr(fPtr);
            total = total + realPtr[0];
        }
    }
    printf("Total for %s: %.2f\n", fName, total);
}

/* Find struct array ARR in MAT-file FILE.
 * Pass field name FIELD to analyzestructarray function. */
int findstructure(
        const char *file,
        const char *arr,
        const char *field) {
    
    MATFile *mfPtr; /* MAT-file pointer */
    mxArray *aPtr;  /* mxArray pointer */
    
    mfPtr = matOpen(file, "r");
    if (mfPtr == NULL) {
        printf("Error opening file %s\n", file);
        return(1);
    }
    
    aPtr = matGetVariable(mfPtr, arr);
    if (aPtr == NULL) {
        printf("mxArray not found: %s\n", arr);
        return(1);
    }
    
    if (mxGetClassID(aPtr) == mxSTRUCT_CLASS) {
        if (mxGetFieldNumber(aPtr, field) == -1) {
            printf("Field not found: %s\n", field);
        }
        else {
            analyzestructarray(aPtr, field);
        }
    } 
    else {
        printf("%s is not a structure\n", arr);
    }
    mxDestroyArray(aPtr);
    
    if (matClose(mfPtr) != 0) {
        printf("Error closing file %s\n", file);
        return(1);
    }
    return(0);
}

int main(int argc, char **argv)
{
/* argv[1] = MAT-file name
 * argv[2] = name of mxArray structure
 * argv[2] = name of field */

    int result;
    if (argc > 3)
        result = findstructure(argv[1], argv[2], argv[3]);
    else {
        result = 0;
        printf("Usage: matreadstructarray <matfile> <struct> <field>\n");
    }
    return (result==0)?EXIT_SUCCESS:EXIT_FAILURE;
}
