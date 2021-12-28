#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include <stdlib.h>

#include "stb_image.h"
#include "ApplicationCommon.h"
#include "EngineCommon.h"

int readImage(char *inputImageFilename, ImageData **imageData);

#endif