#ifndef FILTERS_H
#define FILTERS_H

#include "EngineCommon.h"

#include "None.h"
#include "Multiplier.h"
#include "Monochrome.h"
#include "Red.h"
#include "Green.h"
#include "Blue.h"

Filter *getFilterFromFilterId(FilterID filterId);

#endif