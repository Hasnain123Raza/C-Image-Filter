#ifndef APPLICATION_COMMON_H
#define APPLICATION_COMMON_H

#include <stddef.h>

typedef struct {
    char *value;
    size_t length;
} ApplicationArguments;

typedef struct {
    char *inputImageFilename;
    char *outputImageFilename;
    int filterId;
    ApplicationArguments arguments;
} ApplicationConfigurations;

#endif