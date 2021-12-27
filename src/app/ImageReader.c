#include "ImageReader.h"

int readImage(char *inputImageFilename, ImageData *imageData)
{
    imageData->data = stbi_load(inputImageFilename, &imageData->width, &imageData->height, &imageData->channels, 0);
    if (!imageData->data)
        return 1;

    return 0;
}