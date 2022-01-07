AC_PREREQ([2.69])
AC_INIT([cimgfil], [1.0], [hasnain.mohsan@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

pthread_available=no

# Checks for programs.
AC_PROG_CC

# Checks for command line options
AC_ARG_ENABLE(
    [multithreading],
    [--disable-multithreading],
    [multithreading=${enableval}],
    [multithreading=yes]
)

# Checks for libraries.
AC_SEARCH_LIBS([pthread_create], [pthread], [pthread_available=yes])

# Checks for header files.
if test "x${pthread_available}" = xyes; then
    AC_CHECK_HEADERS([pthread.h], [], [pthread_available=no])
fi

if test "x${pthread_available}" = xno; then
    if test "x${multithreading}" = xyes; then
        AC_MSG_WARN([
            --------------------------------
            Unable to find pthread on this
            system. Building a single-threaded
            version.
            --------------------------------
        ])
    fi
    multithreading=no
fi

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_DEFINE([DEFAULT_INPUT_FILENAME], ["input.jpg"], [The default input filename])
AC_DEFINE([DEFAULT_OUTPUT_FILENAME], ["output.jpg"], [The default output filename])
AC_DEFINE([DEFAULT_FILTER_ID], ["0"], [The default filter])
AC_DEFINE([DEFAULT_THREADS], ["1"], [The default amount of threads])

if test "x${multithreading}" = xyes; then
    AC_DEFINE([MULTITHREADING], [1], [Indicates whether or not to build multithreaded application])
else
    AC_DEFINE([MULTITHREADING], [0], [Indicates whether or not to build multithreaded application])
fi

AC_OUTPUT