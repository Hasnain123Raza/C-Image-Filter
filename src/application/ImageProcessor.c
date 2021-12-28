#include "ImageProcessor.h"

int processImage(ApplicationConfigurations *configurations, ImageData *inputImageData, ImageData **outputImageData)
{
    FilterRequest filterRequest = {
        .filterId = configurations->filterId,
        .arguments = (EngineArguments *)&configurations->arguments
    };

    *outputImageData = applyFilter(&filterRequest, inputImageData);
    if (!(*outputImageData))
        return 1;

    return 0;
}