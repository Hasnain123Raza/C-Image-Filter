#ifndef FILTER_PROCESSOR_H
#define FILTER_PROCESSOR_H

#include <argz.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "EngineCommon.h"
#include "Filters.h"

#if MULTITHREADING == 1
#include <pthread.h>
#endif

ImageData *applyFilter(FilterRequest *filterRequest, ImageData *sourceImageData);

#endif