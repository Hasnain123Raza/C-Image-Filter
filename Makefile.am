bin_PROGRAMS = cimgfil

cimgfil_SOURCES = $(top_srcdir)/src/main.c \
    $(top_srcdir)/src/application/ArgumentParser.c \
    $(top_srcdir)/src/application/ImageReader.c \
    $(top_srcdir)/src/application/ImageProcessor.c \
    $(top_srcdir)/src/application/ImageWriter.c \
    $(top_srcdir)/src/application/stbi/stb_image.c \
    $(top_srcdir)/src/application/stbi/stb_image_write.c \
    $(top_srcdir)/src/engine/FilterProcessor.c \
    $(top_srcdir)/src/engine/Filters.c \
    $(top_srcdir)/src/filters/None.c \
    $(top_srcdir)/src/filters/Multiplier.c \
    $(top_srcdir)/src/filters/Monochrome.c \
    $(top_srcdir)/src/filters/Red.c \
    $(top_srcdir)/src/filters/Green.c \
    $(top_srcdir)/src/filters/Blue.c \
    $(top_srcdir)/src/filters/Yellow.c \
    $(top_srcdir)/src/filters/Cyan.c \
    $(top_srcdir)/src/filters/Magenta.c


EXTRA_DIST = $(top_srcdir)/include/application/ApplicationCommon.h \
    $(top_srcdir)/include/application/ArgumentParser.h \
    $(top_srcdir)/include/application/ImageReader.h \
    $(top_srcdir)/include/application/ImageProcessor.h \
    $(top_srcdir)/include/application/ImageWriter.h \
    $(top_srcdir)/include/application/stbi/stb_image.h \
    $(top_srcdir)/include/application/stbi/stb_image_write.h \
    $(top_srcdir)/include/engine/EngineCommon.h \
    $(top_srcdir)/include/engine/FilterProcessor.h \
    $(top_srcdir)/include/engine/Filters.h \
    $(top_srcdir)/include/filters/None.h \
    $(top_srcdir)/include/filters/Multiplier.h \
    $(top_srcdir)/include/filters/Monochrome.h \
    $(top_srcdir)/include/filters/Red.h \
    $(top_srcdir)/include/filters/Green.h \
    $(top_srcdir)/include/filters/Blue.h \
    $(top_srcdir)/include/filters/Yellow.h \
    $(top_srcdir)/include/filters/Cyan.h \
    $(top_srcdir)/include/filters/Magenta.h


cimgfil_CPPFLAGS = -I$(top_srcdir)/include \
    -I$(top_srcdir)/include/application \
    -I$(top_srcdir)/include/application/stbi \
    -I$(top_srcdir)/include/engine \
    -I$(top_srcdir)/include/filters

cimgfil_LDADD = -lm