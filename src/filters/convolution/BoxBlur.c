#include "BoxBlur.h"

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);

Filter BoxBlur = {
    .filterId = FILTER_BOX_BLUR,
    .configurator = Configurator,
    .function = NULL,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    EngineArguments *arguments = filterRequest->arguments;
    char *kernelSizeArgument = argz_next(arguments->value, arguments->length, NULL);
    char *blurStrengthArgument = kernelSizeArgument ? argz_next(arguments->value, arguments->length, kernelSizeArgument) : NULL;

    if (!kernelSizeArgument || !blurStrengthArgument)
        return 1;

    int kernelSize = atoi(kernelSizeArgument);
    float blurStrength = atof(blurStrengthArgument);

    if (kernelSize < 2)
        return 1;

    if (blurStrength < 0.0f || blurStrength > 1.0f)
        return 1;

    FilterRequest **prefilters = (FilterRequest **)malloc(sizeof(FilterRequest *) * 2);
    if (!prefilters)
        return 1;
    configurations->prefilters = prefilters;
    prefilters[0] = NULL;
    prefilters[1] = NULL;

    FilterRequest *convoluteFilterRequest = (FilterRequest *)malloc(sizeof(FilterRequest));
    if (!convoluteFilterRequest)
    {
        free(prefilters);
        return 1;
    }
    memset(convoluteFilterRequest, 0, sizeof(FilterRequest));

    prefilters[0] = convoluteFilterRequest;
    convoluteFilterRequest->filterId = FILTER_CONVOLUTE;

    float targetPixelValue = 1 / (1 + (kernelSize * kernelSize - 1) * blurStrength);
    float neighbourPixelValue = blurStrength * targetPixelValue;
    int targetPixelBufferLength = snprintf(NULL, 0, "%f", targetPixelValue);
    int neighbourPixelBufferLength = snprintf(NULL, 0, "%f", neighbourPixelValue);
    char *targetPixelBuffer = (char *)malloc(targetPixelBufferLength + 1);
    char *neighbourPixelBuffer = (char *)malloc(neighbourPixelBufferLength + 1);
    if (!targetPixelBuffer || !neighbourPixelBuffer)
    {
        free(prefilters);
        free(convoluteFilterRequest);
        free(targetPixelBuffer);
        free(neighbourPixelBuffer);
        return 1;
    }
    snprintf(targetPixelBuffer, targetPixelBufferLength + 1, "%f", targetPixelValue);
    snprintf(neighbourPixelBuffer, neighbourPixelBufferLength + 1, "%f", neighbourPixelValue);

    EngineArguments *convoluteArguments = (EngineArguments *)malloc(sizeof(EngineArguments));
    if (!convoluteArguments)
    {
        free(prefilters);
        free(convoluteFilterRequest);
        return 1;
    }
    convoluteArguments->length = 0;
    convoluteArguments->value = NULL;
    argz_add(&convoluteArguments->value, &convoluteArguments->length, "2");
    argz_add(&convoluteArguments->value, &convoluteArguments->length, kernelSizeArgument);
    argz_add(&convoluteArguments->value, &convoluteArguments->length, kernelSizeArgument);
    for (int x = 0; x < kernelSize; x++)
    {
        for (int y = 0; y < kernelSize; y++)
        {
            if (x == kernelSize / 2 && y == kernelSize / 2)
                argz_add(&convoluteArguments->value, &convoluteArguments->length, targetPixelBuffer);
            else
                argz_add(&convoluteArguments->value, &convoluteArguments->length, neighbourPixelBuffer);
        }
    }
    convoluteFilterRequest->arguments = convoluteArguments;

    return 0;
}

static int Function(FilterFunctionArguments *arguments)
{
    return 0;
}