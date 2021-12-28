#include "FilterProcessor.h"

static FilterConfigurations *generateFilterConfigurations(FilterConfigurator filterConfigurator, FilterRequest *filterRequest, ImageData *sourceImageData);
static void freeFilterConfigurations(FilterConfigurations *configurations, int ignoreImageData);

static ImageData *cloneImageData(ImageData *sourceImageData);

ImageData *applyFilter(FilterRequest *filterRequest, ImageData *sourceImageData)
{
    Filter *filter = getFilterFromFilterId(filterRequest->filterId);
    if (!filter)
        return NULL;
    
    if (filter->configurator == NULL)
    {
        ImageData *outputImageData = cloneImageData(sourceImageData);
        return outputImageData;
    }

    FilterConfigurations *configurations = generateFilterConfigurations(filter->configurator, filterRequest, sourceImageData);
    if (!configurations)
        return NULL;

    ImageData *prefilteredImageData = sourceImageData;
    if (configurations->prefilters)
    {
        FilterRequest **prefilters = configurations->prefilters;
        while (*prefilters)
        {
            ImageData *temporaryImageData = applyFilter(*prefilters, prefilteredImageData);
            if (!temporaryImageData)
            {
                if (prefilteredImageData != sourceImageData)
                {
                    free(prefilteredImageData->data);
                    free(prefilteredImageData);
                }
                freeFilterConfigurations(configurations, 0);
                return NULL;
            }

            prefilteredImageData = temporaryImageData;
            prefilters++;
        }
    }

    ImageData *targetImageData = configurations->targetImageData;

    if (filter->function == NULL)
    {
        ImageData *outputImageData = prefilteredImageData;
        if (prefilteredImageData == sourceImageData)
            outputImageData = cloneImageData(prefilteredImageData);
        
        freeFilterConfigurations(configurations, 0);
        return outputImageData;
    }

    Chunk chunk = {
        .sourceImageData = sourceImageData,
        .targetImageData = targetImageData,
        .startPixelIndex = 0,
        .endPixelIndex = targetImageData->width * targetImageData->height,
    };

    if (filter->function(&chunk, configurations->userData) != 0)
    {
        if (prefilteredImageData != sourceImageData)
        {
            free(prefilteredImageData->data);
            free(prefilteredImageData);
        }
        freeFilterConfigurations(configurations, 0);
        return NULL;
    }

    if (prefilteredImageData != sourceImageData)
    {
        free(prefilteredImageData->data);
        free(prefilteredImageData);
    }

    if (configurations->postfilters)
    {
        FilterRequest **postfilters = configurations->postfilters;
        while (*postfilters)
        {
            ImageData *temporaryImageData = applyFilter(*postfilters, targetImageData);
            if (!temporaryImageData)
            {
                freeFilterConfigurations(configurations, 0);
                return NULL;
            }

            targetImageData = temporaryImageData;
            postfilters++;
        }
    }

    freeFilterConfigurations(configurations, 1);
    return targetImageData;
}

static FilterConfigurations *generateFilterConfigurations(FilterConfigurator filterConfigurator, FilterRequest *filterRequest, ImageData *sourceImageData)
{
    FilterConfigurations *configurations = (FilterConfigurations *)malloc(sizeof(FilterConfigurations));
    if (!configurations)
        return NULL;

    configurations->prefilters = NULL;
    configurations->postfilters = NULL;
    configurations->userData = NULL;
    configurations->freeUserData = NULL;
    configurations->targetImageData = (ImageData *)malloc(sizeof(ImageData));
    if (!configurations->targetImageData)
    {
        free(configurations);
        return NULL;
    }
    configurations->targetImageData->data = NULL;
    configurations->targetImageData->width = sourceImageData->width;
    configurations->targetImageData->height = sourceImageData->height;
    configurations->targetImageData->channels = sourceImageData->channels;

    if (filterConfigurator(filterRequest, sourceImageData, configurations))
    {
        freeFilterConfigurations(configurations, 0);
        return NULL;
    }

    configurations->targetImageData->data = (unsigned char *)malloc(configurations->targetImageData->width * configurations->targetImageData->height * configurations->targetImageData->channels);
    if (!configurations->targetImageData->data)
    {
        freeFilterConfigurations(configurations, 0);
        return NULL;
    }

    return configurations;
}

static void freeFilterConfigurations(FilterConfigurations *configurations, int ignoreImageData)
{
    if (!configurations)
        return;

    if (configurations->prefilters)
    {
        for (int counter = 0; configurations->prefilters[counter]; counter++)
        {
            FilterRequest *filterRequest = configurations->prefilters[counter];
            EngineArguments *arguments = filterRequest->arguments;
            if (arguments)
            {
                if (argz_count(arguments->value, arguments->length) > 0)
                    free(arguments->value);

                free(arguments);
            }

            free(filterRequest);
        }
        
        free(configurations->prefilters);
    }

    if (configurations->postfilters)
    {
        for (int counter = 0; configurations->postfilters[counter]; counter++)
        {
            FilterRequest *filterRequest = configurations->postfilters[counter];
            EngineArguments *arguments = filterRequest->arguments;
            if (arguments)
            {
                if (argz_count(arguments->value, arguments->length) > 0)
                    free(arguments->value);

                free(arguments);
            }

            free(filterRequest);
        }
        
        free(configurations->postfilters);
    }

    if (!ignoreImageData && configurations->targetImageData)
    {
        if (configurations->targetImageData->data)
            free(configurations->targetImageData->data);
        free(configurations->targetImageData);
    }

    if (configurations->freeUserData)
        if (configurations->userData)
            configurations->freeUserData(configurations->userData);

    free(configurations);
}

static ImageData *cloneImageData(ImageData *sourceImageData)
{
    ImageData *outputImageData = (ImageData *)malloc(sizeof(ImageData));
    if (!outputImageData)
        return NULL;

    int outputImageDataSize = sizeof(unsigned char) * sourceImageData->width * sourceImageData->height * sourceImageData->channels;
    outputImageData->data = (unsigned char *)malloc(outputImageDataSize);
    if (!outputImageData->data)
    {
        free(outputImageData);
        return NULL;
    }

    outputImageData->width = sourceImageData->width;
    outputImageData->height = sourceImageData->height;
    outputImageData->channels = sourceImageData->channels;

    memcpy(outputImageData->data, sourceImageData->data, outputImageDataSize);

    return outputImageData;
}