/* Copyright 2017 The MathWorks, Inc. */

#ifndef StreamingProfilerTgtAppSvcCInterf_h
#define StreamingProfilerTgtAppSvcCInterf_h

#include "StreamingProfilerTgtAppSvc_dll.hpp"
#include <stddef.h>

#if defined(_MSC_VER) && (_MSC_VER < 1600)
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#elif defined(__LCC__)
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#else
#include <stdint.h>
#endif

STREAMINGPROFILERTGTAPPSVC_API_C void uploadProfileData(const void *aTaskID, const void *aCoreID, const void *aTimerValue);
STREAMINGPROFILERTGTAPPSVC_API_C void initializeProfilingAppSvc();

#endif
