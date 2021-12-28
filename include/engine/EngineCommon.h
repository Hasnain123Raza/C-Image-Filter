#ifndef ENGINE_COMMON_H
#define ENGINE_COMMON_H

#include <stddef.h>

typedef enum {
    FILTER_NONE,
    FILTER_MONOCHROME,
    FILTER_RED,
    FILTER_GREEN,
    FILTER_BLUE,
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

typedef int (*FilterFunction)(Chunk *chunk, void *userData);

typedef struct Filter {
    FilterID filterId;
    FilterConfigurator configurator;
    FilterFunction function;
} Filter;

#endif