#include <stdio.h>
#include <stdlib.h>

#include "ApplicationCommon.h"
#include "ArgumentParser.h"
#include "EngineCommon.h"

#include "ImageReader.h"
#include "ImageProcessor.h"
#include "ImageWriter.h"

static void freeApplicationConfigurations(ApplicationConfigurations *configurations);
static void freeImageData(ImageData *imageData);

int main(int argc, char *argv[])
{
    ApplicationConfigurations configurations;

    int parse = parseArguments(argc, argv, &configurations);

    if (parse != 0)
    {
        freeApplicationConfigurations(&configurations);
        return 1;
    }

    ImageData *inputImageData = NULL;
    ImageData *outputImageData = NULL;

    if (readImage(configurations.inputImageFilename, &inputImageData) != 0)
    {
        freeApplicationConfigurations(&configurations);
        return 1;
    }

    if (processImage(&configurations, inputImageData, &outputImageData) != 0)
    {
        freeImageData(inputImageData);
        freeApplicationConfigurations(&configurations);
        return 1;
    }

    if (writeImage(configurations.outputImageFilename, outputImageData) != 0)
    {
        freeImageData(inputImageData);
        freeImageData(outputImageData);
        freeApplicationConfigurations(&configurations);
        return 1;
    }

    freeImageData(inputImageData);
    freeImageData(outputImageData);
    freeApplicationConfigurations(&configurations);

    return 0;
}

static void freeApplicationConfigurations(ApplicationConfigurations *configurations)
{
    if (argz_count(configurations->arguments.value, configurations->arguments.length) > 0)
        free(configurations->arguments.value);
}

static void freeImageData(ImageData *imageData)
{
    if (imageData)
    {
        if (imageData->data)
            free(imageData->data);
        free(imageData);
    }
}