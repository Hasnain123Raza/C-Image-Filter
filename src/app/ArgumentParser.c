#include "ArgumentParser.h"

const char *argp_program_bug_address = PACKAGE_BUGREPORT;
const char *argp_program_version = PACKAGE_STRING;

static int parse_opt(int key, char *arg, struct argp_state *state);

int parseArguments(int argc, char *argv[], Configurations *configurations)
{
    static struct argp_option options[] = {
        { "input", 'i', "FILENAME", OPTION_ARG_OPTIONAL, "Filename of input. Defaults to " DEFAULT_INPUT_FILENAME},
        { "output", 'o', "FILENAME", OPTION_ARG_OPTIONAL, "Filename of output. Defaults to " DEFAULT_OUTPUT_FILENAME},
        { "filter", 'f', "FILTERID", OPTION_ARG_OPTIONAL, "ID of filter to use. Defaults to " DEFAULT_FILTER_ID},
        { 0 }
    };

    static struct argp argp = {
        options, parse_opt, "[ARGUMENTS...]", 0, 0, 0, 0
    };

    configurations->inputImageFilename = DEFAULT_INPUT_FILENAME;
    configurations->outputImageFilename = DEFAULT_OUTPUT_FILENAME;
    configurations->filterId = atoi(DEFAULT_FILTER_ID);
    configurations->arguments.value = 0;
    configurations->arguments.length = 0;

    return argp_parse(&argp, argc, argv, 0, 0, (void *)configurations);
}

static int parse_opt(int key, char *arg, struct argp_state *state)
{
    Configurations *configurations = (Configurations *)state->input;

    switch (key) {
        case 'i':
            configurations->inputImageFilename = arg;
            break;
        case 'o':
            configurations->outputImageFilename = arg;
            break;
        case 'f':
            configurations->filterId = atoi(arg);
            break;
        case ARGP_KEY_ARG:
            argz_add(&configurations->arguments.value, &configurations->arguments.length, arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}