#ifndef FILTERS_H
#define FILTERS_H

#include "EngineCommon.h"

#include "None.h"
#include "Multiplier.h"
#include "Monochrome.h"
#include "Red.h"
#include "Green.h"
#include "Blue.h"
#include "Yellow.h"
#include "Cyan.h"
#include "Magenta.h"
#include "Sepia.h"
#include "Negative.h"
#include "Grayscale.h"
#include "Brightness.h"
#include "Saturation.h"
#include "RotateClockwise.h"
#include "RotateCounterClockwise.h"
#include "FlipVertical.h"
#include "FlipHorizontal.h"
#include "Convolute.h"
#include "BoxBlur.h"
#include "Sharpen.h"

Filter *getFilterFromFilterId(FilterID filterId);

#endif