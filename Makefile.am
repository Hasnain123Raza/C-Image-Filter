bin_PROGRAMS = cimgfil

cimgfil_SOURCES = $(top_srcdir)/src/main.c \
    $(top_srcdir)/src/app/ArgumentParser.c \
    $(top_srcdir)/src/app/ImageReader.c \
    $(top_srcdir)/src/app/ImageProcessor.c \
    $(top_srcdir)/src/app/ImageWriter.c \
    $(top_srcdir)/src/app/stbi/stb_image.c \
    $(top_srcdir)/src/app/stbi/stb_image_write.c

EXTRA_DIST = $(top_srcdir)/include/app/AppCommon.h \
    $(top_srcdir)/include/app/ArgumentParser.h \
    $(top_srcdir)/include/app/ImageReader.h \
    $(top_srcdir)/include/app/ImageProcessor.h \
    $(top_srcdir)/include/app/ImageWriter.h \
    $(top_srcdir)/include/app/stbi/stb_image.h \
    $(top_srcdir)/include/app/stbi/stb_image_write.h \
    $(top_srcdir)/include/engine/EngineCommon.h


cimgfil_CPPFLAGS = -I$(top_srcdir)/include \
    -I$(top_srcdir)/include/app \
    -I$(top_srcdir)/include/app/stbi \
    -I$(top_srcdir)/include/engine

cimgfil_LDADD = -lm