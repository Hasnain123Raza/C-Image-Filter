#ifndef ARGUEMENT_PARSER_H
#define ARGUEMENT_PARSER_H

#include <argp.h>
#include <argz.h>
#include <stdlib.h>

#include "config.h"
#include "ApplicationCommon.h"

int parseArguments(int argc, char *argv[], ApplicationConfigurations *configurations);

#endif