/* Copyright 2013 MathWorks, Inc. */

#ifndef ToAsyncQueueTgtAppSvcCIntrf_h
#define ToAsyncQueueTgtAppSvcCIntrf_h

#include "ToAsyncQueueTgtAppSvc_dll.hpp"

#if defined(_MSC_VER) && (_MSC_VER < 1600)
typedef unsigned short uint16_t;
#elif defined(__LCC__)
typedef unsigned short uint16_t;
#else
#include <stdint.h>
#endif

TOASYNCQUEUETGTAPPSVC_API_C int  startToAsyncQueueTgtAppSvc();
TOASYNCQUEUETGTAPPSVC_API_C int  terminateToAsyncQueueTgtAppSvc();
TOASYNCQUEUETGTAPPSVC_API_C void sendToAsyncQueueTgtAppSvc(uint16_t id, double time, void *data, uint16_t size);

#endif
