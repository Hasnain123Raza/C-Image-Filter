#include "FilterProcessor.h"

static int scheduleProcessing(FilterFunction filterFunction, ImageData *sourceImageData, ImageData *targetImageData, void *userData, FilterRequest *filterRequest);

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
#if MULTITHREADING == 1
            if ((*prefilters)->threads == 0)
                (*prefilters)->threads = filterRequest->threads;
#endif
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

    if (scheduleProcessing(filter->function, sourceImageData, targetImageData, configurations->userData, filterRequest) != 0)
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
#if MULTITHREADING == 1
            if ((*postfilters)->threads == 0)
                (*postfilters)->threads = filterRequest->threads;
#endif
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

static int scheduleProcessing(FilterFunction filterFunction, ImageData *sourceImageData, ImageData *targetImageData, void *userData, FilterRequest *filterRequest)
{
#if MULTITHREADING == 0
    Chunk chunk = {
        .sourceImageData = sourceImageData,
        .targetImageData = targetImageData,
        .startPixelIndex = 0,
        .endPixelIndex = targetImageData->width * targetImageData->height,
    };

    FilterFunctionArguments arguments = {
        .chunk = &chunk,
        .userData = userData,
    };

    return filterFunction(&arguments);
#else
    int imageSize = targetImageData->width * targetImageData->height;
    int actualThreads = filterRequest->threads > imageSize ? imageSize : filterRequest->threads;
    int threadSize = (imageSize + actualThreads - 1) / actualThreads;

    pthread_t *threads = (pthread_t *)malloc(actualThreads * sizeof(pthread_t));
    if (!threads)
        return 1;
    
    Chunk *chunks = (Chunk *)malloc(actualThreads * sizeof(Chunk));
    if (!chunks)
    {
        free(threads);
        return 1;
    }

    FilterFunctionArguments *arguments = (FilterFunctionArguments *)malloc(actualThreads * sizeof(FilterFunctionArguments));
    if (!arguments)
    {
        free(threads);
        free(chunks);
        return 1;
    }

    for (int index = 0; index < actualThreads; index++)
    {
        int startPixelIndex = index * threadSize;
        int endPixelIndex = (index + 1) * threadSize;
        if (endPixelIndex > imageSize)
            endPixelIndex = imageSize;
        if (index == actualThreads - 1)
            endPixelIndex = imageSize;

        chunks[index] = (Chunk) {
            .sourceImageData = sourceImageData,
            .targetImageData = targetImageData,
            .startPixelIndex = startPixelIndex,
            .endPixelIndex = endPixelIndex,
        };

        arguments[index] = (FilterFunctionArguments) {
            .chunk = &chunks[index],
            .userData = userData,
        };

        if (pthread_create(&threads[index], NULL, (void *(*)(void *))filterFunction, &arguments[index]) != 0)
        {
            for (int counter = 0; counter < index; counter++)
                pthread_cancel(threads[counter]);
            free(threads);
            free(chunks);
            return 1;
        }

        if (endPixelIndex == imageSize)
        {
            actualThreads = index + 1;
            break;
        }
    }

    int status = 0;

    for (int index = 0; index < actualThreads; index++)
        status = pthread_join(threads[index], NULL);

    free(threads);
    free(chunks);

    return status;
#endif
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