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

## Building and Installation

Clone this repository and navigate to the project root directory. Run `mkdir build && cd build` then run `../configure` then run `make` then run `make install`. You should be able to run the program using cimgfil. You can uninstall it by running `make uninstall`.