#include "ImageProcessor.h"

static double tick();

int processImage(ApplicationConfigurations *configurations, ImageData *inputImageData, ImageData **outputImageData)
{
    FilterRequest filterRequest = {
        .filterId = configurations->filterId,
        .arguments = (EngineArguments *)&configurations->arguments,
#if MULTITHREADING == 1
        .threads = configurations->threads,
#endif
    };

    double startTime = tick();
    *outputImageData = applyFilter(&filterRequest, inputImageData);
    double endTime = tick();
    if (!(*outputImageData))
        return 1;

    printf("Processed image in %.3f seconds\n", endTime - startTime);

    return 0;
}

static double tick()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}