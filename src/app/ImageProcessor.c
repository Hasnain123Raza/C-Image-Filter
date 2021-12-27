#include "ImageProcessor.h"

int processImage(Configurations *configurations, ImageData *inputImageData, ImageData *outputImageData)
{
    outputImageData->width = inputImageData->width;
    outputImageData->height = inputImageData->height;
    outputImageData->channels = inputImageData->channels;

    outputImageData->data = malloc(outputImageData->width * outputImageData->height * outputImageData->channels);
    if (!outputImageData->data)
        return 1;
    
    memcpy(outputImageData->data, inputImageData->data, outputImageData->width * outputImageData->height * outputImageData->channels);

    return 0;
}