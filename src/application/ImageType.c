#include "ImageType.h"

ImageType getImageTypeFromFilename(char *filename)
{
    int length = strlen(filename);
    int extensionIndex = -1;

    for (int index = length - 1; index >= 0; index--)
    {
        if (filename[index] == '.')
        {
            extensionIndex = index;
            break;
        }
    }

    if (extensionIndex == -1)
        return IMAGE_TYPE_UNKNOWN;

    char *extension = &filename[extensionIndex + 1];
    if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0)
        return IMAGE_TYPE_JPEG;
    else if (strcmp(extension, "png") == 0)
        return IMAGE_TYPE_PNG;
    else
        return IMAGE_TYPE_UNKNOWN;
}