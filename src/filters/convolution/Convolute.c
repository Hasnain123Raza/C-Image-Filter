#include "Convolute.h"

typedef enum {
    EDGE_MODE_EXTEND,
    EDGE_MODE_WRAP,
    EDGE_MODE_MIRROR,
    TOTAL_EDGE_MODE
} EdgeMode;

typedef struct {
    EdgeMode edgeMode;
    int rows;
    int columns;
    float *kernel;
} ConvoluteUserData;

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);
static int Function(FilterFunctionArguments *arguments);
static void FreeUserData(void *userData);

Filter Convolute = {
    .filterId = FILTER_CONVOLUTE,
    .configurator = Configurator,
    .function = Function,
};

static int Configurator(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations)
{
    ConvoluteUserData *userData = (ConvoluteUserData *)malloc(sizeof(ConvoluteUserData));
    if (!userData)
        return 1;
    
    EngineArguments *arguments = filterRequest->arguments;
    char *edgeModeArgument = argz_next(arguments->value, arguments->length, NULL);
    char *rowsArgument = edgeModeArgument ? argz_next(arguments->value, arguments->length, edgeModeArgument) : NULL;
    char *columnsArgument = rowsArgument ? argz_next(arguments->value, arguments->length, rowsArgument) : NULL;

    int edgeMode = atoi(edgeModeArgument);
    if (edgeMode < 0 || edgeMode >= TOTAL_EDGE_MODE)
        edgeMode = EDGE_MODE_EXTEND;
    int rows = atoi(rowsArgument);
    int columns = atoi(columnsArgument);

    if (rows < 2 || columns < 2)
    {
        free(userData);
        return 1;
    }

    int kernelSize = rows * columns;
    float *kernel = (float *)malloc(kernelSize * sizeof(float));
    if (!kernel)
    {
        free(userData);
        return 1;
    }

    char *currentArgument = columnsArgument;
    for (int counter = 0; counter < kernelSize; counter++)
    {
        char *value = currentArgument ? argz_next(arguments->value, arguments->length, currentArgument) : NULL;
        kernel[counter] = value ? atof(value) : 0.0f;
        currentArgument = value;
    }

    userData->edgeMode = edgeMode;
    userData->rows = rows;
    userData->columns = columns;
    userData->kernel = kernel;

    configurations->userData = userData;
    configurations->freeUserData = FreeUserData;

    return 0;
}

static int Function(FilterFunctionArguments *arguments)
{
    Chunk *chunk = arguments->chunk;
    void *userData = arguments->userData;

    ConvoluteUserData *convoluteUserData = (ConvoluteUserData *)userData;
    int edgeMode = convoluteUserData->edgeMode;
    int rows = convoluteUserData->rows;
    int columns = convoluteUserData->columns;
    float *kernel = convoluteUserData->kernel;

    unsigned char *sourceData = chunk->sourceImageData->data;
    unsigned char *targetData = chunk->targetImageData->data;
    int channels = chunk->sourceImageData->channels;

    int sourceWidth = chunk->sourceImageData->width;
    int sourceHeight = chunk->sourceImageData->height;

    for (int index = chunk->startPixelIndex; index < chunk->endPixelIndex; index++)
    {
        int sourceX = index % sourceWidth;
        int sourceY = index / sourceWidth;

        float channelValues[channels];
        memset(channelValues, 0, sizeof(float) * channels);

        for (int kernelY = 0; kernelY < rows; kernelY++)
        {
            int sourceYOffset = sourceY + kernelY - (rows / 2);
            

            for (int kernelX = 0; kernelX < columns; kernelX++)
            {
                int sourceXOffset = sourceX + kernelX - (columns / 2);

                switch (edgeMode)
                {
                    case EDGE_MODE_EXTEND:
                        {
                            if (sourceYOffset < 0)
                                sourceYOffset = 0;
                            else if (sourceYOffset >= sourceHeight)
                                sourceYOffset = sourceHeight - 1;
                            
                            if (sourceXOffset < 0)
                                sourceXOffset = 0;
                            else if (sourceXOffset >= sourceWidth)
                                sourceXOffset = sourceWidth - 1;
                        }
                        break;

                    case EDGE_MODE_WRAP:
                        {
                            if (sourceYOffset < 0)
                                sourceYOffset = sourceHeight - (abs(sourceYOffset) % sourceHeight);
                            else if (sourceYOffset >= sourceHeight)
                                sourceYOffset = sourceYOffset % sourceHeight;
                            
                            if (sourceXOffset < 0)
                                sourceXOffset = sourceWidth - (abs(sourceXOffset) % sourceWidth);
                            else if (sourceXOffset >= sourceWidth)
                                sourceXOffset = sourceXOffset % sourceWidth;
                        }
                        break;
                    
                    case EDGE_MODE_MIRROR:
                        {
                            if (sourceYOffset < 0)
                            {
                                sourceYOffset = (2 * sourceHeight) - (abs(sourceYOffset) % (2 * sourceHeight));
                                if (sourceYOffset >= sourceHeight)
                                    sourceYOffset = (2 * sourceHeight) - sourceYOffset;
                            }
                            else if (sourceYOffset >= sourceHeight)
                            {
                                sourceYOffset = sourceYOffset % (2 * sourceHeight);
                                if (sourceYOffset >= sourceHeight)
                                    sourceYOffset = (2 * sourceHeight) - sourceYOffset - 1;
                            }

                            if (sourceXOffset < 0)
                            {
                                sourceXOffset = (2 * sourceWidth) - (abs(sourceXOffset) % (2 * sourceWidth));
                                if (sourceXOffset >= sourceWidth)
                                    sourceXOffset = (2 * sourceWidth) - sourceXOffset;
                            }
                            else if (sourceXOffset >= sourceWidth)
                            {
                                sourceXOffset = sourceXOffset % (2 * sourceWidth);
                                if (sourceXOffset >= sourceWidth)
                                    sourceXOffset = (2 * sourceWidth) - sourceXOffset - 1;
                            }
                        }
                        break;
                }

                int sourceIndex = sourceYOffset * sourceWidth + sourceXOffset;
                int kernelIndex = kernelY * columns + kernelX;

                for (int channel = 0; channel < channels; channel++)
                    channelValues[channel] += sourceData[sourceIndex * channels + channel] * kernel[kernelIndex];
            }
        }

        for (int channel = 0; channel < channels; channel++)
        {
            if (channelValues[channel] > 255.0f)
                channelValues[channel] = 255.0f;
            else if (channelValues[channel] < 0.0f)
                channelValues[channel] = 0.0f;

            targetData[index * channels + channel] = (unsigned char)channelValues[channel];
        }
    }

    return 0;
}

static void FreeUserData(void *userData)
{
    ConvoluteUserData *convoluteUserData = (ConvoluteUserData *)userData;
    free(convoluteUserData->kernel);
    free(convoluteUserData);
}