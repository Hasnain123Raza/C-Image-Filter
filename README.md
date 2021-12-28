# C Image Filter

## Introduction

This is a C program for modifying images using some filters.

Note: This program has only been tested with jpg images with three channels. For the time being, please only input jpg images with three channels.

## Implementation Details

There are three main components to this program: the command line application, the main engine, and the filters. The source is also organized according to those main components. The philosophy is that the engine and the filters are separated from the command line application so that porting to a graphics application is much easier. The command line application uses the execellent stbi header-only library to read and write the image data into a file (https://github.com/nothings/stb). The engine applies different filters to transform the loaded image data.

## Usage

The program has a few arguments that can be summarized using `-?` or `--help` arguments.

### Input Filename

> -i, --input[=FILENAME]

This argument is used to specify the input filename. It is optional and defaults to `input.jpg`.

### Output Filename

> -o, --output[=FILENAME]

This argument is used to specify the output filename. It is optional and defaults to `output.jpg`.

### Filter

> -f, --filter[=FILTERID]

This argument is used to specify the filter to use. It is optional and defaults to `0`.

The avaialable filters are as follows:

0   FILTER_NONE

1   FILTER_MULTIPLIER

2   FILTER_MONOCHROME

3   FILTER_RED

4   FILTER_GREEN

5   FILTER_BLUE

6   FILTER_YELLOW

7   FILTER_CYAN

8   FILTER_MAGENTA

9   FILTER_SEPIA

10  FILTER_NEGATIVE

11  FILTER_GRAYSCALE

12  FILTER_BRIGHTNESS

13  FILTER_SATURATION

### Filter Arguments

> [arguments...]

These are optional arguments that are passed over to the filters.

## Building and Installing

Clone this repository and navigate to the project root directory. Run `mkdir build && cd build` then run `../configure` then run `make` then run `make install`. You should be able to run the program using cimgfil. You can uninstall it by running `make uninstall`.

## Contribution

The main focus of contribution is on the development of additional filters. For that reason, it is important to understand what exactly a filter is and how the engine uses the filters to transform the source image data.

### Filter Transform Function

A filter has two important functions associated with it. Firstly, it has the transform function that iterates over the target image data, reads the source image data, and writes the transformed values. For example, a grayscale filters would simply average the red, green, and blue components and write the gray value. Note, a transform function is not mandatory and the benefits of leaving it out will become apparent soon.

### Filter Configurator Function

The second function is called the configurator. The configurator is used to parse command line arguments that are sent to the filter, generate custom user data to be used during the transform function, and also set attributes of the target image such as width, height, and channels. For example, a grayscale filter would specify in its configurator that it's target should have only one channel instead of three. The configurator can also create prefilters and postfilters. Note, a configurator function is not mandatory but it doesn't make sense to make a filter without one.

### Prefilters and Postfilters

Prefilters and postfilters are filter requests generated by a filter's configurator. These are used to invoke other filters before and after applying a filter's own transform function. It is usually a good idea to create general filters and specify them as prefilters to more specific filters to avoid repetitive code. For example, when implementing red, green, blue, yellow, cyan, and mangenta filters, instead of writing each of them with a transform function that does basically the same thing, it is much better to implement a monochrome filter that transforms each rgb component using multipliers and specify this filter as a prefilter with different arguments to each of the aforementioned filters. This leaves only one transform function to maintain because the more specific filters do not need a transform function anymore.

### Data Structures

For common data structures used in the engine, please refer to `include/engine/EngineCommon.h`.

## Creating Filters

1: To create a new filter, start by going into `include/engine/EngineCommon.h` and add your filter's ID to the `FilterID` enum.

2: Next, copy `include/filters/None.h` and paste it to `include/filters/{YourFilterName}.h`. In the header file, change the header guard and the extern variable name appropriately (same name as your filter's).

3: Next, copy `src/filters/None.c` and paste it to `src/filters/{YourFilterName}.c`. In the source file, change the include preprocessor directive and the Filter variable appropriately (same name as your filter's).

4: Navigate to `include/engine/Filters.h` and add an include preprocessor directive to include your filter's header file.

5: Navigate to `src/engine/Filters.c` and augment the case statement to properly return your filter.

6: Finally, go into `Makefile.am` and add your filter's source file to `cimgfil_SOURCES` and your filter's header file to `EXTRA_DIST`.