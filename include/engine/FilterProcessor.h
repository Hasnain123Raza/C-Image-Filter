#ifndef FILTER_PROCESSOR_H
#define FILTER_PROCESSOR_H

#include <argz.h>
#include <stdlib.h>
#include <string.h>

#include "EngineCommon.h"
#include "Filters.h"

ImageData *applyFilter(FilterRequest *filterRequest, ImageData *sourceImageData);

#endif