#include "ImageReader.h"

int readImage(char *inputImageFilename, ImageData **imageData)
{
    *imageData = (ImageData *)malloc(sizeof(ImageData));
    if (!(*imageData))
        return 1;

    (*imageData)->data = stbi_load(inputImageFilename, &(*imageData)->width, &(*imageData)->height, &(*imageData)->channels, 0);
    if (!(*imageData)->data)
    {
        free(*imageData);
        return 1;
    }

    return 0;
}