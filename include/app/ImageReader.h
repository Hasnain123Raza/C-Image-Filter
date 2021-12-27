#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include "stb_image.h"
#include "AppCommon.h"
#include "EngineCommon.h"

int readImage(char *inputImageFilename, ImageData *imageData);

#endif