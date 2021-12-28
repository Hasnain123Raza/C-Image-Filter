#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <stdlib.h>
#include <string.h>

#include "ApplicationCommon.h"
#include "EngineCommon.h"
#include "FilterProcessor.h"

int processImage(ApplicationConfigurations *configurations, ImageData *inputImageData, ImageData **outputImageData);

#endif