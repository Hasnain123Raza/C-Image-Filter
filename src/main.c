#include <stdio.h>

#include "AppCommon.h"
#include "ArgumentParser.h"
#include "EngineCommon.h"

#include "ImageReader.h"
#include "ImageProcessor.h"
#include "ImageWriter.h"

int main(int argc, char *argv[])
{
    Configurations configurations;

    int parse = parseArguments(argc, argv, &configurations);

    if (parse != 0)
    {
        if (argz_count(configurations.arguments.value, configurations.arguments.length) > 0)
            free(configurations.arguments.value);
        
        return 1;
    }

    ImageData inputImage = {};
    ImageData outputImage = {};

    if (readImage(configurations.inputImageFilename, &inputImage) != 0)
    {
        if (argz_count(configurations.arguments.value, configurations.arguments.length) > 0)
            free(configurations.arguments.value);
        
        return 1;
    }

    if (processImage(&configurations, &inputImage, &outputImage) != 0)
    {
        free(inputImage.data);
        if (outputImage.data)
            free(outputImage.data);
        if (argz_count(configurations.arguments.value, configurations.arguments.length) > 0)
            free(configurations.arguments.value);
        
        return 1;
    }

    if (writeImage(configurations.outputImageFilename, &outputImage) != 0)
    {
        free(inputImage.data);
        free(outputImage.data);
        if (argz_count(configurations.arguments.value, configurations.arguments.length) > 0)
            free(configurations.arguments.value);
        
        return 1;
    }

    free(inputImage.data);
    free(outputImage.data);
    if (argz_count(configurations.arguments.value, configurations.arguments.length) > 0)
        free(configurations.arguments.value);

    return 0;
}