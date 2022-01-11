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
    char *blurSizeArgument = argz_next(arguments->value, arguments->length, NULL);
    char *blurStrengthArgument = blurSizeArgument ? argz_next(arguments->value, arguments->length, blurSizeArgument) : NULL;

    if (!blurSizeArgument || !blurStrengthArgument)
        return 1;

    int blurSize = atoi(blurSizeArgument);
    float blurStrength = atof(blurStrengthArgument);

    if (blurSize < 2)
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

    int kernelSize = blurSize * blurSize;
    float neighbourPixelValue = (1.0f / kernelSize) * blurStrength;
    float targetPixelValue = 1 - (kernelSize - 1) * neighbourPixelValue;
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
    argz_add(&convoluteArguments->value, &convoluteArguments->length, blurSizeArgument);
    argz_add(&convoluteArguments->value, &convoluteArguments->length, blurSizeArgument);
    for (int x = 0; x < blurSize; x++)
    {
        for (int y = 0; y < blurSize; y++)
        {
            if (x == blurSize / 2 && y == blurSize / 2)
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