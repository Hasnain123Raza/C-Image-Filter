#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H

#include "stb_image_write.h"
#include "ApplicationCommon.h"
#include "EngineCommon.h"

int writeImage(char *outputImageFilename, ImageData *imageData);

#endif