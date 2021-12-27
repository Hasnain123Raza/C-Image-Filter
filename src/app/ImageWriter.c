#include "ImageWriter.h"

int writeImage(char *outputImageFilename, ImageData *imageData)
{
    if (stbi_write_jpg(outputImageFilename, imageData->width, imageData->height, imageData->channels, imageData->data, 100) == 0)
        return 1;
    
    return 0;
}