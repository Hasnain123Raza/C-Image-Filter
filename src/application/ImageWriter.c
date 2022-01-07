#include "ImageWriter.h"

int writeImage(char *outputImageFilename, ImageData *imageData)
{
    ImageType imageType = getImageTypeFromFilename(outputImageFilename);

    unsigned char *data = imageData->data;
    int width = imageData->width;
    int height = imageData->height;
    int channels = imageData->channels;

    switch (imageType)
    {
        case IMAGE_TYPE_PNG:
            return stbi_write_png(outputImageFilename, width, height, channels, data, channels * width) == 0;
        case IMAGE_TYPE_JPEG:
            return stbi_write_jpg(outputImageFilename, width, height, channels, data, 100) == 0;
        default:
            return 1;
    }
}