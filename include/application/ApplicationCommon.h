#ifndef APPLICATION_COMMON_H
#define APPLICATION_COMMON_H

#include <stddef.h>

#include "config.h"

typedef struct {
    char *value;
    size_t length;
} ApplicationArguments;

typedef struct {
    char *inputImageFilename;
    char *outputImageFilename;
    int filterId;
#if MULTITHREADING == 1
    int threads;
#endif
    ApplicationArguments arguments;
} ApplicationConfigurations;

#endif