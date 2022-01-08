bin_PROGRAMS = cimgfil

cimgfil_SOURCES = $(top_srcdir)/src/main.c \
    $(top_srcdir)/src/application/ArgumentParser.c \
    $(top_srcdir)/src/application/ImageType.c \
    $(top_srcdir)/src/application/ImageReader.c \
    $(top_srcdir)/src/application/ImageProcessor.c \
    $(top_srcdir)/src/application/ImageWriter.c \
    $(top_srcdir)/src/application/stbi/stb_image.c \
    $(top_srcdir)/src/application/stbi/stb_image_write.c \
    $(top_srcdir)/src/engine/FilterProcessor.c \
    $(top_srcdir)/src/engine/Filters.c \
    $(top_srcdir)/src/filters/None.c \
    $(top_srcdir)/src/filters/multiplication/Multiplier.c \
    $(top_srcdir)/src/filters/multiplication/Monochrome.c \
    $(top_srcdir)/src/filters/multiplication/Red.c \
    $(top_srcdir)/src/filters/multiplication/Green.c \
    $(top_srcdir)/src/filters/multiplication/Blue.c \
    $(top_srcdir)/src/filters/multiplication/Yellow.c \
    $(top_srcdir)/src/filters/multiplication/Cyan.c \
    $(top_srcdir)/src/filters/multiplication/Magenta.c \
    $(top_srcdir)/src/filters/multiplication/Sepia.c \
    $(top_srcdir)/src/filters/multiplication/Negative.c \
    $(top_srcdir)/src/filters/Grayscale.c \
    $(top_srcdir)/src/filters/Brightness.c \
    $(top_srcdir)/src/filters/Saturation.c \
    $(top_srcdir)/src/filters/edition/RotateClockwise.c \
    $(top_srcdir)/src/filters/edition/RotateCounterClockwise.c \
    $(top_srcdir)/src/filters/edition/FlipVertical.c \
    $(top_srcdir)/src/filters/edition/FlipHorizontal.c \
    $(top_srcdir)/src/filters/convolution/Convolute.c


EXTRA_DIST = $(top_srcdir)/include/application/ApplicationCommon.h \
    $(top_srcdir)/include/application/ArgumentParser.h \
    $(top_srcdir)/include/application/ImageType.h \
    $(top_srcdir)/include/application/ImageReader.h \
    $(top_srcdir)/include/application/ImageProcessor.h \
    $(top_srcdir)/include/application/ImageWriter.h \
    $(top_srcdir)/include/application/stbi/stb_image.h \
    $(top_srcdir)/include/application/stbi/stb_image_write.h \
    $(top_srcdir)/include/engine/EngineCommon.h \
    $(top_srcdir)/include/engine/FilterProcessor.h \
    $(top_srcdir)/include/engine/Filters.h \
    $(top_srcdir)/include/filters/None.h \
    $(top_srcdir)/include/filters/multiplication/Multiplier.h \
    $(top_srcdir)/include/filters/multiplication/Monochrome.h \
    $(top_srcdir)/include/filters/multiplication/Red.h \
    $(top_srcdir)/include/filters/multiplication/Green.h \
    $(top_srcdir)/include/filters/multiplication/Blue.h \
    $(top_srcdir)/include/filters/multiplication/Yellow.h \
    $(top_srcdir)/include/filters/multiplication/Cyan.h \
    $(top_srcdir)/include/filters/multiplication/Magenta.h \
    $(top_srcdir)/include/filters/multiplication/Sepia.h \
    $(top_srcdir)/include/filters/multiplication/Negative.h \
    $(top_srcdir)/include/filters/Grayscale.h \
    $(top_srcdir)/include/filters/Brightness.h \
    $(top_srcdir)/include/filters/Saturation.h \
    $(top_srcdir)/include/filters/edition/RotateClockwise.h \
    $(top_srcdir)/include/filters/edition/RotateCounterClockwise.h \
    $(top_srcdir)/include/filters/edition/FlipVertical.h \
    $(top_srcdir)/include/filters/edition/FlipHorizontal.h \
    $(top_srcdir)/include/filters/convolution/Convolute.h


cimgfil_CPPFLAGS = -I$(top_srcdir)/include \
    -I$(top_srcdir)/include/application \
    -I$(top_srcdir)/include/application/stbi \
    -I$(top_srcdir)/include/engine \
    -I$(top_srcdir)/include/filters \
    -I$(top_srcdir)/include/filters/multiplication \
    -I$(top_srcdir)/include/filters/edition \
    -I$(top_srcdir)/include/filters/convolution

cimgfil_LDADD = -lm