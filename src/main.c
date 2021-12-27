#include "AppCommon.h"
#include "ArgumentParser.h"

int main(int argc, char *argv[])
{
    Configurations configurations;

    int parse = parseArguments(argc, argv, &configurations);

    if (parse != 0)
    {
        if (argz_count(configurations.arguments.value, configurations.arguments.length) > 0)
            free(configurations.arguments.value);
        
        return parse;
    }

    if (argz_count(configurations.arguments.value, configurations.arguments.length) > 0)
        free(configurations.arguments.value);

    return 0;
}