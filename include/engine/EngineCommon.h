#ifndef ENGINE_COMMON_H
#define ENGINE_COMMON_H

#include <stddef.h>

#include "config.h"

typedef enum {
    FILTER_NONE,
    FILTER_MULTIPLIER,
    FILTER_MONOCHROME,
    FILTER_RED,
    FILTER_GREEN,
    FILTER_BLUE,
    FILTER_YELLOW,
    FILTER_CYAN,
    FILTER_MAGENTA,
    FILTER_SEPIA,
    FILTER_NEGATIVE,
    FILTER_GRAYSCALE,
    FILTER_BRIGHTNESS,
    FILTER_SATURATION,
    FILTER_ROTATE_CLOCKWISE,
    FILTER_ROTATE_COUNTERCLOCKWISE,
    FILTER_FLIP_VERTICAL,
    FILTER_FLIP_HORIZONTAL,
    TOTAL_FILTERS
} FilterID;

typedef struct {
    char *value;
    size_t length;
} EngineArguments;

typedef struct {
    unsigned char *data;
    int width;
    int height;
    int channels;
} ImageData;

typedef struct {
    FilterID filterId;
    EngineArguments *arguments;
#if MULTITHREADING == 1
    int threads;
#endif
} FilterRequest;

typedef struct {
    FilterRequest **prefilters;
    FilterRequest **postfilters;
    ImageData *targetImageData;
    void *userData;
    void (*freeUserData)(void *userData);
} FilterConfigurations;

typedef int (*FilterConfigurator)(FilterRequest *filterRequest, ImageData *sourceImageData, FilterConfigurations *configurations);

typedef struct {
    ImageData *sourceImageData;
    ImageData *targetImageData;
    int startPixelIndex;
    int endPixelIndex;
} Chunk;

typedef struct {
    Chunk *chunk;
    void *userData;
} FilterFunctionArguments;

typedef int (*FilterFunction)(FilterFunctionArguments *arguments);

typedef struct Filter {
    FilterID filterId;
    FilterConfigurator configurator;
    FilterFunction function;
} Filter;

#endif