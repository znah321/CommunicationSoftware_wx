/*
 * Read a cell array CELL from MATFILE and
 * display class for each element.
 *
 * Calling syntax:
 *
 *   matreadcellarray <matfile> <cell>
 *
 * See the MATLAB External Interfaces Guide for 
 * compiling information.
 *
 * Copyright 2012 The MathWorks, Inc.
 */

#include <stdlib.h>
#include "mat.h"

/* Access each cell in cell array CPTR. */
static void
analyzecellarray(const mxArray *cPtr)
{
    mwSize nCells;          /* number of cells in array */
    mwIndex cIdx;           /* cell index */
    const mxArray *ePtr;    /* element pointer */
    mxClassID category;     /* class ID */
    
    nCells = (mwSize)mxGetNumberOfElements(cPtr);
    for (cIdx = 0; cIdx < nCells; cIdx++) {
        ePtr = mxGetCell(cPtr, cIdx);
        if (ePtr == NULL) {
            printf("Empty Cell\n");
            break;
        }
        category = mxGetClassID(ePtr);
        printf("%d: ", cIdx);
        switch (category) {
            case mxCHAR_CLASS:
                printf("string\n");
                /* see revord.c */
                break;
            case mxSTRUCT_CLASS:
                printf("structure\n");
                /* see analyzestructure.c */
                break;
            case mxCELL_CLASS:
                printf("cell\n");
                printf("{\n");
                analyzecellarray(ePtr);
                printf("}\n");
                break;
            case mxUNKNOWN_CLASS:
                printf("Unknown class\n");
                break;
            default:
                if (mxIsSparse(ePtr)) {
                    printf("sparse array\n");
                    /* see mxsetnzmax.c */
                } 
                else {
                    printf("numeric class\n");
                }
                break;
        }
    }
}

/* Find cell array ARR in MAT-file FILE. */
int findcell(const char *file, const char *arr) {
    
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
    
    if (mxGetClassID(aPtr) == mxCELL_CLASS) {
        analyzecellarray(aPtr);
    } 
    else {
        printf("%s is not a cell array\n", arr);
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
 * argv[2] = name of mxArray cell array */

    int result;
    if (argc > 2)
        result = findcell(argv[1], argv[2]);
    else {
        result = 0;
        printf("Usage: matreadcellarray <matfile> <cell>\n");
    }
    return (result==0)?EXIT_SUCCESS:EXIT_FAILURE;
}
